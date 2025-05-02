// SPDX-License-Identifier: GPL-3.0-or-later

f_frame
f_create(void)
{
	return (f_frame)
	{
		.buf = calloc(1, sizeof(e_char)),
		.cap = 1,
		.hist = calloc(1, sizeof(f_histentry)),
		.histcap = 1
	};
}

// assumes that s is non-empty.
f_frame
f_fromstr(char const *s)
{
	usize buflen;
	e_char *buf = e_fromstr(&buflen, s);
	return (f_frame)
	{
		.buf = buf,
		.len = buflen,
		.cap = buflen,
		.hist = calloc(1, sizeof(f_histentry)),
		.histcap = 1
	};
}

i32
f_fromfile(OUT f_frame *f, char const *file)
{
	FILE *fp = fopen(file, "rb");
	if (!fp)
	{
		showerr("frame: failed to read file - %s!", file);
		return 1;
	}
	
	e_char *buf = calloc(1, sizeof(e_char));
	u32 buflen = 0, bufcap = 1;
	
	for (;;)
	{
		e_char ch = e_fread(fp);
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
		
		if (ch.codepoint == E_INVALIDCODEPOINT)
		{
			showerr("frame: file contained an unusable character - %s!", file);
			fclose(fp);
			return 1;
		}
		
		if (buflen >= bufcap)
		{
			bufcap *= 2;
			buf = reallocarray(buf, bufcap, sizeof(e_char));
		}
		
		buf[buflen++] = ch;
	}
	
	fclose(fp);
	
	*f = (f_frame)
	{
		.buf = buf,
		.len = buflen,
		.cap = bufcap,
		.hist = calloc(1, sizeof(f_histentry)),
		.histcap = 1
	};
	return 0;
}

void
f_destroy(f_frame *f)
{
	free(f->buf);
	free(f->hist);
	if (f->src)
	{
		free(f->src);
	}
}

void
f_render(f_frame const *f, u32 x, u32 y, u32 w, u32 h)
{
	// TODO: implement.
}

i32
f_save(f_frame *f)
{
	if (!(f->flags & F_UNSAVED))
	{
		return 0;
	}
	
	if (!f->src)
	{
		// TODO: implement file chooser save logic.
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
f_writech(f_frame *f, e_char ch, u32 pos)
{
	f_write(f, &ch, pos, 1);
}

void
f_write(f_frame *f, e_char *data, u32 pos, usize n)
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
		f->buf = reallocarray(f->buf, f->cap, sizeof(e_char));
	}
	
	memmove(&f->buf[pos + n], &f->buf[pos], sizeof(e_char) * (f->len - pos));
	memcpy(&f->buf[n], data, sizeof(e_char) * n);
	f->len += n;
	f->flags |= F_UNSAVED;
	
	// push history entry.
	f_histentry *prev = f->histlen ? &f->hist[f->histlen - 1] : NULL;
	if (prev && prev->type == F_WRITE && prev->write.ub == pos)
	{
		prev->write.ub = pos + n;
	}
	else
	{
		if (f->histlen >= f->histcap)
		{
			f->histcap *= 2;
			f->hist = reallocarray(f->hist, f->histcap, sizeof(f_histentry));
		}
		
		f->hist[f->histlen++] = (f_histentry)
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
f_erase(f_frame *f, u32 lb, u32 ub)
{
	// TODO: implement.
}

void
f_undo(f_frame *f)
{
	// TODO: implement.
}

void
f_breakhist(f_frame *f)
{
	if (f->len >= f->cap)
	{
		f->cap *= 2;
		f->hist = reallocarray(f->hist, f->cap, sizeof(f_histentry));
	}
	
	f->hist[f->len++] = (f_histentry)
	{
		.type = F_BREAK
	};
}
