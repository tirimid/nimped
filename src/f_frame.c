// SPDX-License-Identifier: GPL-3.0-or-later

f_frame_t
f_create(void)
{
	return (f_frame_t)
	{
		.buf = calloc(1, sizeof(e_char_t)),
		.cap = 1,
		.hist = calloc(1, sizeof(f_hist_t)),
		.histcap = 1
	};
}

// assumes that s is non-empty.
f_frame_t
f_fromstr(char const *s)
{
	usize buflen;
	e_char_t *buf = e_fromstr(&buflen, s);
	return (f_frame_t)
	{
		.buf = buf,
		.len = buflen,
		.cap = buflen,
		.hist = calloc(1, sizeof(f_hist_t)),
		.histcap = 1
	};
}

i32
f_fromfile(OUT f_frame_t *f, char const *file)
{
	FILE *fp = fopen(file, "rb");
	if (!fp)
	{
		showerr("frame: failed to read file - %s!", file);
		return 1;
	}
	
	e_char_t *buf = calloc(1, sizeof(e_char_t));
	u32 buflen = 0, bufcap = 1;
	
	for (;;)
	{
		e_char_t ch = e_fread(fp);
		if (feof(fp))
		{
			break;
		}
		
		if (ferror(fp))
		{
			showerr("frame: experienced a read failure for file - %s!", file);
			fclose(fp);
			return 1;
		}
		
		if (buflen >= bufcap)
		{
			bufcap *= 2;
			buf = hreallocarray(buf, bufcap, sizeof(e_char_t));
		}
		
		buf[buflen++] = ch;
	}
	
	fclose(fp);
	
	*f = (f_frame_t)
	{
		.buf = buf,
		.len = buflen,
		.cap = bufcap,
		.src = strdup(file),
		.hist = calloc(1, sizeof(f_hist_t)),
		.histcap = 1
	};
	return 0;
}

void
f_destroy(f_frame_t *f)
{
	free(f->buf);
	free(f->hist);
	if (f->src)
	{
		free(f->src);
	}
}

void
f_render(f_frame_t const *f, u32 x, u32 y, u32 w, u32 h, bool active)
{
	// render window top bar.
	r_attr_t atop = active ? (r_attr_t){o_opts.curwndfg, o_opts.curwndbg} : (r_attr_t){o_opts.wndfg, o_opts.wndbg};
	r_fill(e_fromcodepoint(' '), atop, x, y, w, 1);
	
	usize namelen;
	e_char_t *name = e_fromstr(&namelen, f->src ? f->src : O_SCRATCHNAME);
	
	{
		u32 i;
		for (i = 0; i < namelen && i < w; ++i)
		{
			r_putch(name[i], x + i, y);
		}
		
		if (f->flags & F_UNSAVED)
		{
			i -= i >= w;
			r_putch(e_fromcodepoint('*'), x + i, y);
		}
	}
	
	free(name);
	
	// fill frame and gutter.
	u32 startline = 1;
	for (u32 i = 0; i < f->start; ++i)
	{
		startline += f->buf[i].codepoint == '\n';
	}
	
	u32 lastline = 1;
	for (u32 i = 0; i < f->len; ++i)
	{
		lastline += f->buf[i].codepoint == '\n';
	}
	
	u32 linumlen = 0;
	while (lastline)
	{
		++linumlen;
		lastline /= 10;
	}
	
	r_fill(
		e_fromcodepoint(' '),
		(r_attr_t){o_opts.linumfg, o_opts.linumbg},
		x,
		y + 1,
		linumlen + o_opts.lgutter + o_opts.rgutter,
		h - 1
	);
	
	r_fill(
		e_fromcodepoint(' '),
		(r_attr_t){o_opts.normfg, o_opts.normbg},
		x + linumlen + o_opts.lgutter + o_opts.rgutter,
		y + 1,
		w - linumlen + o_opts.lgutter + o_opts.rgutter,
		h - 1
	);
	
	u32 leftpad = o_opts.lgutter + linumlen + o_opts.rgutter;
	
	// render margin.
	if (leftpad + o_opts.margin < w)
	{
		r_fill(
			e_fromcodepoint(O_MARGINCHAR),
			(r_attr_t){o_opts.marginfg, o_opts.marginbg},
			x + leftpad + o_opts.margin,
			y + 1,
			1,
			h - 1
		);
	}
	
	// render frame contents and find cursor pos.
	u32 cx = 0, cy = 0;
	u32 csrx = -1, csry = -1;
	u32 curline = startline;
	
	h_region_t hlreg = {0};
	h_find(&hlreg, f, 0);
	while (hlreg.ub < f->start)
	{
		h_find(&hlreg, f, hlreg.ub);
	}
	
	for (u32 i = f->start; i < f->len; ++i)
	{
		if (i >= hlreg.ub)
		{
			h_find(&hlreg, f, hlreg.ub);
		}
		
		if (!cx)
		{
			char linum[32];
			snprintf(linum, sizeof(linum), "%u", curline);
			
			u32 pad = o_opts.lgutter + linumlen - strlen(linum);
			for (u32 j = 0; linum[j]; ++j)
			{
				r_putch(e_fromcodepoint(linum[j]), x + pad + j, y + cy + 1);
			}
			
			++curline;
		}
		
		if (leftpad + cx >= w)
		{
			cx = 0;
			++cy;
		}
		
		if (cy + 1 >= h)
		{
			break;
		}
		
		if (i == f->csr)
		{
			csrx = cx;
			csry = cy;
		}
		
		u32 cw;
		switch (f->buf[i].codepoint)
		{
		case '\n':
			cx = 0;
			++cy;
			continue;
		case '\t':
			cw = o_opts.tab - cx % o_opts.tab;
			break;
		default:
			cw = 1;
			if (i >= hlreg.lb && i < hlreg.ub)
			{
				r_putattr(
					(r_attr_t){hlreg.fg, hlreg.bg},
					x + leftpad + cx,
					y + cy + 1
				);
			}
			else
			{
				r_putattr(
					(r_attr_t){o_opts.normfg, o_opts.normbg},
					x + leftpad + cx,
					y + cy + 1
				);
			}
			r_putch(
				e_isprint(f->buf[i]) ? f->buf[i] : e_fromcodepoint(E_REPLACEMENT),
				x + leftpad + cx,
				y + cy + 1
			);
			break;
		}
		
		cx += cw;
	}
	
	csrx = csrx == (u32)-1 ? cx : csrx;
	csry = csry == (u32)-1 ? cy : csry;
	
	// render cursor and row highlights.
	r_fillattr((r_attr_t){o_opts.linumhlfg, o_opts.linumhlbg}, x, y + csry + 1, leftpad, 1);
	for (u32 i = 0; leftpad + i < w; ++i)
	{
		r_attr_t a = r_getattr(x + leftpad + i, y + csry + 1);
		a.bg = a.bg == o_opts.normbg ? o_opts.csrhlbg : a.bg;
		r_putattr(a, x + leftpad + i, y + csry + 1);
	}
	r_putattr((r_attr_t){o_opts.csrfg, o_opts.csrbg}, x + leftpad + csrx, y + csry + 1);
}

i32
f_save(f_frame_t *f)
{
	if (!f->src)
	{
		showerr("frame: cannot save frame with no source!");
		return 1;
	}
	
	FILE *fp = fopen(f->src, "wb");
	if (!fp)
	{
		showerr("frame: failed to open file source for writing - %s!", f->src);
		return 1;
	}
	
	for (usize i = 0; i < f->len; ++i)
	{
		if (e_fputch(f->buf[i], fp))
		{
			showerr("frame: failed to write file, take care not to lose data - %s!", f->src);
			fclose(fp);
			return 1;
		}
	}
	
	fclose(fp);
	f->flags &= ~F_UNSAVED;
	
	return 0;
}

void
f_writech(f_frame_t *f, e_char_t ch, u32 pos)
{
	f_write(f, &ch, pos, 1);
}

void
f_write(f_frame_t *f, e_char_t const *data, u32 pos, usize n)
{
	// modify buffer.
	u32 newcap = f->cap;
	for (usize i = 1; i <= n; ++i)
	{
		if (f->len + i > newcap)
		{
			newcap *= 2;
		}
	}
	
	if (newcap != f->cap)
	{
		f->cap = newcap;
		f->buf = hreallocarray(f->buf, f->cap, sizeof(e_char_t));
	}
	
	hmemmove(&f->buf[pos + n], &f->buf[pos], sizeof(e_char_t) * (f->len - pos));
	hmemcpy(&f->buf[pos], data, sizeof(e_char_t) * n);
	f->len += n;
	f->flags |= F_UNSAVED;
	
	// push history entry.
	f_hist_t *h = f->histlen ? &f->hist[f->histlen - 1] : NULL;
	if (h && h->type == F_WRITE && h->write.ub == pos)
	{
		h->write.ub = pos + n;
	}
	else
	{
		if (f->histlen >= f->histcap)
		{
			f->histcap *= 2;
			f->hist = hreallocarray(f->hist, f->histcap, sizeof(f_hist_t));
		}
		
		f->hist[f->histlen++] = (f_hist_t)
		{
			.write =
			{
				.type = F_WRITE,
				.lb = pos,
				.ub = pos + n
			}
		};
	}
}

void
f_erase(f_frame_t *f, u32 lb, u32 ub)
{
	// push history entry.
	f_hist_t *h = f->histlen ? &f->hist[f->histlen - 1] : NULL;
	if (h && h->type == F_ERASE && h->erase.lb == ub)
	{
		h->erase.data = hreallocarray(h->erase.data, h->erase.ub - lb, sizeof(e_char_t));
		hmemmove(&h->erase.data[ub - lb], h->erase.data, sizeof(e_char_t) * (h->erase.ub - h->erase.lb));
		hmemcpy(h->erase.data, &f->buf[lb], sizeof(e_char_t) * (ub - lb));
		h->erase.lb = lb;
	}
	else
	{
		if (f->histlen >= f->histcap)
		{
			f->histcap *= 2;
			f->hist = hreallocarray(f->hist, f->histcap, sizeof(f_hist_t));
		}
		
		e_char_t *data = calloc(ub - lb, sizeof(e_char_t));
		hmemcpy(data, &f->buf[lb], sizeof(e_char_t) * (ub - lb));
		f->hist[f->histlen++] = (f_hist_t)
		{
			.erase =
			{
				.type = F_ERASE,
				.lb = lb,
				.ub = ub,
				.data = data
			}
		};
	}
	
	// modify buffer.
	hmemmove(&f->buf[lb], &f->buf[ub], sizeof(e_char_t) * (f->len - ub));
	f->len -= ub - lb;
	f->flags |= F_UNSAVED;
}

void
f_undo(f_frame_t *f)
{
	while (f->histlen && f->hist[f->histlen - 1].type == F_BREAK)
	{
		--f->histlen;
	}
	
	if (!f->histlen)
	{
		return;
	}
	
	f_hist_t const *h = &f->hist[f->histlen - 1];
	switch (h->type)
	{
	case F_WRITE:
		hmemmove(
			&f->buf[h->write.lb],
			&f->buf[h->write.ub],
			sizeof(e_char_t) * (f->len - h->write.ub)
		);
		f->len -= h->write.ub - h->write.lb;
		f->csr = h->write.lb;
		f->flags |= F_UNSAVED;
		break;
	case F_ERASE:
		hmemmove(
			&f->buf[h->erase.ub],
			&f->buf[h->erase.lb],
			sizeof(e_char_t) * (f->len - h->erase.lb)
		);
		hmemcpy(
			&f->buf[h->erase.lb],
			h->erase.data,
			sizeof(e_char_t) * (h->erase.ub - h->erase.lb)
		);
		f->len += h->erase.ub - h->erase.lb;
		f->csr = h->erase.ub;
		f->flags |= F_UNSAVED;
		free(h->erase.data);
		break;
	}
	
	--f->histlen;
}

void
f_breakhist(f_frame_t *f)
{
	if (f->len >= f->cap)
	{
		f->cap *= 2;
		f->hist = hreallocarray(f->hist, f->cap, sizeof(f_hist_t));
	}
	
	f->hist[f->len++] = (f_hist_t)
	{
		.type = F_BREAK
	};
}

void
f_savecsr(f_frame_t *f)
{
	u32 lbegin = f->csr;
	while (lbegin > 0 && f->buf[lbegin - 1].codepoint != '\n')
	{
		--lbegin;
	}
	
	u32 cx = 0;
	for (u32 i = lbegin; i < f->csr; ++i)
	{
		switch (f->buf[i].codepoint)
		{
		case '\t':
			cx += o_opts.tab - cx % o_opts.tab;
			break;
		default:
			++cx;
			break;
		}
	}
	
	f->svcsrx = cx;
}

void
f_loadcsr(f_frame_t *f)
{
	u32 lbegin = f->csr;
	while (lbegin > 0 && f->buf[lbegin - 1].codepoint != '\n')
	{
		--lbegin;
	}
	
	u32 i = lbegin;
	for (u32 cx = 0; i < f->len && f->buf[i].codepoint != '\n' && cx < f->svcsrx; ++i)
	{
		switch (f->buf[i].codepoint)
		{
		case '\t':
			cx += o_opts.tab - cx % o_opts.tab;
			break;
		default:
			++cx;
			break;
		}
	}
	
	f->csr = i;
}

void
f_compbounds(f_frame_t *f, u32 w, u32 h)
{
	if (f->csr < f->start)
	{
		f->start = f->csr;
		while (f->start > 0 && f->buf[f->start - 1].codepoint != '\n')
		{
			--f->start;
		}
		return;
	}
	
	u32 lastline = 1;
	for (u32 i = 0; i < f->len; ++i)
	{
		lastline += f->buf[i].codepoint == '\n';
	}
	
	u32 linumlen = 0;
	while (lastline)
	{
		++linumlen;
		lastline /= 10;
	}
	
	u32 cx = 0, cy = 0;
	for (usize i = f->start; i < f->csr; ++i)
	{
		if (o_opts.lgutter + o_opts.rgutter + linumlen + cx >= w)
		{
			cx = 0;
			++cy;
		}
		
		u32 cw;
		switch (f->buf[i].codepoint)
		{
		case '\n':
			cx = 0;
			++cy;
			continue;
		case '\t':
			cw = o_opts.tab - cx % o_opts.tab;
			break;
		default:
			cw = 1;
			break;
		}
		
		cx += cw;
	}
	
	for (cx = 0; cy + 1 >= h; ++f->start)
	{
		if (o_opts.lgutter + o_opts.rgutter + linumlen + cx >= w)
		{
			cx = 0;
			--cy;
		}
		
		u32 cw;
		switch (f->buf[f->start].codepoint)
		{
		case '\n':
			cx = 0;
			--cy;
			continue;
		case '\t':
			cw = o_opts.tab - cx % o_opts.tab;
			break;
		default:
			cw = 1;
			break;
		}
		
		cx += cw;
	}
}
