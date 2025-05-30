// SPDX-License-Identifier: GPL-3.0-or-later

static void b_fmvleft(void);
static void b_fmvright(void);
static void b_fmvup(void);
static void b_fmvdown(void);
static void b_fmvstart(void);
static void b_fmvend(void);
static void b_fmvwordleft(void);
static void b_fmvwordright(void);
static void b_pmvleft(void);
static void b_pmvright(void);
static void b_pmvstart(void);
static void b_pmvend(void);
static void b_pmvwordleft(void);
static void b_pmvwordright(void);
static void b_quit(void);
static void b_quitpromptfail(void);
static void b_quitpromptsuccess(void);
static void b_next(void);
static void b_prev(void);
static void b_fdelfront(void);
static void b_fdelback(void);
static void b_fdelword(void);
static void b_pdelfront(void);
static void b_pdelback(void);
static void b_pdelword(void);
static void b_newline(void);
static void b_undo(void);
static void b_newframe(void);
static void b_killframe(void);
static void b_save(void);
static void b_focus(void);
static void b_openfile(void);
static void b_search(void);
static void b_revsearch(void);
static void b_fleftparen(void);
static void b_fleftbracket(void);
static void b_fleftbrace(void);
static void b_fdoublequote(void);
static void b_pleftparen(void);
static void b_pleftbracket(void);
static void b_pleftbrace(void);
static void b_pdoublequote(void);
static void b_paste(void);
static void b_copyline(void);
static void b_cutline(void);
static void b_ncopyline(void);
static void b_ncutline(void);
static void b_zoom(void);
static void b_goto(void);
static void b_recmacro(void);
static void b_execmacro(void);

void
b_installbase(void)
{
	i_unbind();
	i_bind(o_bfmvleft, b_fmvleft);
	i_bind(o_bfmvright, b_fmvright);
	i_bind(o_bfmvup, b_fmvup);
	i_bind(o_bfmvdown, b_fmvdown);
	i_bind(o_bfmvstart, b_fmvstart);
	i_bind(o_bfmvend, b_fmvend);
	i_bind(o_bfmvwordleft, b_fmvwordleft);
	i_bind(o_bfmvwordright, b_fmvwordright);
	i_bind(o_bquit, b_quit);
	i_bind(o_bnext, b_next);
	i_bind(o_bprev, b_prev);
	i_bind(o_bwritemode, b_installwrite);
	i_bind(o_bundo, b_undo);
	i_bind(o_bnewframe, b_newframe);
	i_bind(o_bkillframe, b_killframe);
	i_bind(o_bsave, b_save);
	i_bind(o_bfocus, b_focus);
	i_bind(o_bopenfile, b_openfile);
	i_bind(o_bsearch, b_search);
	i_bind(o_brevsearch, b_revsearch);
	i_bind(o_bpaste, b_paste);
	i_bind(o_bcopyline, b_copyline);
	i_bind(o_bcutline, b_cutline);
	i_bind(o_bncopyline, b_ncopyline);
	i_bind(o_bncutline, b_ncutline);
	i_bind(o_bzoom, b_zoom);
	i_bind(o_bgoto, b_goto);
	i_bind(o_brecmacro, b_recmacro);
	i_bind(o_bexecmacro, b_execmacro);
	i_organize();
	
	w_state.writeinput = false;
	
	r_setbarstr(O_BASENAME);
}

void
b_installwrite(void)
{
	i_unbind();
	i_bind(o_bquit, b_installbase);
	i_bind(o_bdelfront, b_fdelfront);
	i_bind(o_bdelback, b_fdelback);
	i_bind(o_bdelword, b_fdelword);
	i_bind(o_bnewline, b_newline);
	i_bind(o_bleftparen, b_fleftparen);
	i_bind(o_bleftbracket, b_fleftbracket);
	i_bind(o_bleftbrace, b_fleftbrace);
	i_bind(o_bdoublequote, b_fdoublequote);
	i_organize();
	
	w_state.writeinput = true;
	
	r_setbarstr(O_WRITENAME);
}

void
b_installprompt(void)
{
	i_unbind();
	i_bind(o_bpcancel3, b_quitpromptfail);
	i_bind(o_bpcancel2, b_quitpromptfail);
	i_bind(o_bpcancel1, b_quitpromptfail);
	i_bind(o_bnewline, b_quitpromptsuccess);
	i_bind(o_bpmvleft, b_pmvleft);
	i_bind(o_bpmvright, b_pmvright);
	i_bind(o_bpmvstart, b_pmvstart);
	i_bind(o_bpmvend, b_pmvend);
	i_bind(o_bpmvwordleft, b_pmvwordleft);
	i_bind(o_bpmvwordright, b_pmvwordright);
	i_bind(o_bdelfront, b_pdelfront);
	i_bind(o_bdelback, b_pdelback);
	i_bind(o_bdelword, b_pdelword);
	i_bind(o_bleftparen, b_pleftparen);
	i_bind(o_bleftbracket, b_pleftbracket);
	i_bind(o_bleftbrace, b_pleftbrace);
	i_bind(o_bdoublequote, b_pdoublequote);
	i_organize();
	
	w_state.writeinput = false;
}

void
b_installpathprompt(void)
{
	i_unbind();
	i_bind(o_bpcancel3, b_quitpromptfail);
	i_bind(o_bpcancel2, b_quitpromptfail);
	i_bind(o_bpcancel1, b_quitpromptfail);
	i_bind(o_bnewline, b_quitpromptsuccess);
	i_bind(o_bpmvleft, b_pmvleft);
	i_bind(o_bpmvright, b_pmvright);
	i_bind(o_bpmvstart, b_pmvstart);
	i_bind(o_bpmvend, b_pmvend);
	i_bind(o_bpmvwordleft, b_pmvwordleft);
	i_bind(o_bpmvwordright, b_pmvwordright);
	i_bind(o_bdelfront, b_pdelfront);
	i_bind(o_bdelback, b_pdelback);
	i_bind(o_bdelword, b_pdelword);
	i_bind(o_bcomplete, p_pathcomplete);
	i_bind(o_bleftparen, b_pleftparen);
	i_bind(o_bleftbracket, b_pleftbracket);
	i_bind(o_bleftbrace, b_pleftbrace);
	i_bind(o_bdoublequote, b_pdoublequote);
	i_organize();
	
	w_state.writeinput = false;
}

void
b_installconfirmprompt(void)
{
	i_unbind();
	i_bind(o_byes, b_quitpromptsuccess);
	i_bind(o_bno, b_quitpromptfail);
	i_bind(o_bcancel, b_quitpromptfail);
	i_organize();
	
	w_state.writeinput = false;
}

void
b_installnumberprompt(void)
{
	i_unbind();
	i_bind(o_bpcancel3, b_quitpromptfail);
	i_bind(o_bpcancel2, b_quitpromptfail);
	i_bind(o_bpcancel1, b_quitpromptfail);
	i_bind(o_bnewline, b_quitpromptsuccess);
	i_bind(o_bpmvleft, b_pmvleft);
	i_bind(o_bpmvright, b_pmvright);
	i_bind(o_bpmvstart, b_pmvstart);
	i_bind(o_bpmvend, b_pmvend);
	i_bind(o_bpmvwordleft, b_pmvwordleft);
	i_bind(o_bpmvwordright, b_pmvwordright);
	i_bind(o_bdelfront, b_pdelfront);
	i_bind(o_bdelback, b_pdelback);
	i_bind(o_bdelword, b_pdelword);
	i_organize();
	
	w_state.writeinput = false;
}

static void
b_fmvleft(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->csr > 0)
	{
		--f->csr;
		f_savecsr(f);
	}
}

static void
b_fmvright(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->csr < f->len)
	{
		++f->csr;
		f_savecsr(f);
	}
}

static void
b_fmvup(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	f->csr -= f->csr > 0;
	while (f->csr > 0 && f->buf[f->csr].codepoint != '\n')
	{
		--f->csr;
	}
	f_loadcsr(f);
}

static void
b_fmvdown(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr < f->len && f->buf[f->csr].codepoint != '\n')
	{
		++f->csr;
	}
	f->csr += f->csr < f->len;
	f_loadcsr(f);
}

static void
b_fmvstart(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr > 0 && f->buf[f->csr - 1].codepoint != '\n')
	{
		--f->csr;
	}
	f_savecsr(f);
}

static void
b_fmvend(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr < f->len && f->buf[f->csr].codepoint != '\n')
	{
		++f->csr;
	}
	f_savecsr(f);
}

static void
b_fmvwordleft(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr && !e_isalnum(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	while (f->csr && e_isalnum(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	f_savecsr(f);
}

static void
b_fmvwordright(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr < f->len && !e_isalnum(f->buf[f->csr]))
	{
		++f->csr;
	}
	while (f->csr < f->len && e_isalnum(f->buf[f->csr]))
	{
		++f->csr;
	}
	f_savecsr(f);
}

static void
b_pmvleft(void)
{
	p_prompt.csr -= (u32)p_prompt.csr > p_prompt.start;
}

static void
b_pmvright(void)
{
	p_prompt.csr += (u32)p_prompt.csr < p_prompt.len;
}

static void
b_pmvstart(void)
{
	p_prompt.csr = p_prompt.start;
}

static void
b_pmvend(void)
{
	p_prompt.csr = p_prompt.len;
}

static void
b_pmvwordleft(void)
{
	while ((u32)p_prompt.csr > p_prompt.start
		&& !e_isalnum(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
	while ((u32)p_prompt.csr > p_prompt.start
		&& e_isalnum(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
}

static void
b_pmvwordright(void)
{
	while ((u32)p_prompt.csr < p_prompt.len
		&& !e_isalnum(p_prompt.data[p_prompt.csr]))
	{
		++p_prompt.csr;
	}
	while ((u32)p_prompt.csr < p_prompt.len
		&& e_isalnum(p_prompt.data[p_prompt.csr]))
	{
		++p_prompt.csr;
	}
}

static void
b_quit(void)
{
	for (usize i = 0; i < w_state.nframes; ++i)
	{
		if (!(w_state.frames[i].flags & F_UNSAVED))
		{
			continue;
		}
		
		b_installconfirmprompt();
		p_beginstr("frames have unsaved changes, quit anyway? (y/n)");
		p_prompt.csr = -1;
		while(p_prompt.rc == 0)
		{
			w_render();
			p_render();
			r_present();
			
			i_readkey();
		}
		p_end();
		
		if (p_prompt.rc == P_FAIL)
		{
			b_installbase();
			return;
		}
		
		break;
	}
	
	w_state.running = false;
}

static void
b_quitpromptfail(void)
{
    fprintf(stderr, "Prompt failed");
    fflush(stderr);
	p_prompt.rc = P_FAIL;
}

static void
b_quitpromptsuccess(void)
{
	p_prompt.rc = P_SUCCESS;
}

static void
b_next(void)
{
	w_state.curframe = w_state.curframe == w_state.nframes - 1 ? 0 : w_state.curframe + 1;
}

static void
b_prev(void)
{
	w_state.curframe = w_state.curframe == 0 ? w_state.nframes - 1 : w_state.curframe - 1;
}

static void
b_fdelfront(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->csr < f->len)
	{
		f_erase(f, f->csr, f->csr + 1);
	}
}

static void
b_fdelback(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (!f->csr)
	{
		return;
	}
	
	if (f->csr < f->len)
	{
		if ((f->buf[f->csr - 1].codepoint == '(' && f->buf[f->csr].codepoint == ')')
			|| (f->buf[f->csr - 1].codepoint == '[' && f->buf[f->csr].codepoint == ']')
			|| (f->buf[f->csr - 1].codepoint == '{' && f->buf[f->csr].codepoint == '}')
			|| (f->buf[f->csr - 1].codepoint == '"' && f->buf[f->csr].codepoint == '"'))
		{
			--f->csr;
			f_erase(f, f->csr, f->csr + 2);
			f_savecsr(f);
			return;
		}
	}
	
	--f->csr;
	f_erase(f, f->csr, f->csr + 1);
	f_savecsr(f);
}

static void
b_fdelword(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	u32 ub = f->csr;
	while (f->csr && !e_isalnum(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	while (f->csr && e_isalnum(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	f_savecsr(f);
	
	if (f->csr < ub)
	{
		f_erase(f, f->csr, ub);
	}
}

static void
b_pdelfront(void)
{
	if ((u32)p_prompt.csr < p_prompt.len)
	{
		p_erase(p_prompt.csr, p_prompt.csr + 1);
	}
}

static void
b_pdelback(void)
{
	if ((u32)p_prompt.csr <= p_prompt.start)
	{
		return;
	}
	
	if ((u32)p_prompt.csr < p_prompt.len)
	{
		if ((p_prompt.data[p_prompt.csr - 1].codepoint == '(' && p_prompt.data[p_prompt.csr].codepoint == ')')
			|| (p_prompt.data[p_prompt.csr - 1].codepoint == '[' && p_prompt.data[p_prompt.csr].codepoint == ']')
			|| (p_prompt.data[p_prompt.csr - 1].codepoint == '{' && p_prompt.data[p_prompt.csr].codepoint == '}')
			|| (p_prompt.data[p_prompt.csr - 1].codepoint == '"' && p_prompt.data[p_prompt.csr].codepoint == '"'))
		{
			--p_prompt.csr;
			p_erase(p_prompt.csr, p_prompt.csr + 2);
			return;
		}
	}
	
	--p_prompt.csr;
	p_erase(p_prompt.csr, p_prompt.csr + 1);
}

static void
b_pdelword(void)
{
	u32 ub = p_prompt.csr;
	while ((u32)p_prompt.csr > p_prompt.start
		&& !e_isalnum(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
	while ((u32)p_prompt.csr > p_prompt.start
		&& e_isalnum(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
	
	if ((u32)p_prompt.csr < ub)
	{
		p_erase(p_prompt.csr, ub);
	}
}

static void
b_newline(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	u32 lbegin = f->csr;
	while (lbegin && f->buf[lbegin - 1].codepoint != '\n')
	{
		--lbegin;
	}
	
	u32 ntab = 0;
	while (lbegin + ntab < f->len && f->buf[lbegin + ntab].codepoint == '\t')
	{
		++ntab;
	}
	
	// unfold parentheticals.
	bool unfolded = false;
	if (f->csr && f->csr < f->len)
	{
		u32 prevcp = f->buf[f->csr - 1].codepoint, curcp = f->buf[f->csr].codepoint;
		
		if (prevcp == '(' || prevcp == '[' || prevcp == '{')
		{
			f_writech(f, e_fromcodepoint('\n'), f->csr);
			++f->csr;
			
			for (u32 i = 0; i < ntab + 1; ++i)
			{
				f_writech(f, e_fromcodepoint('\t'), f->csr);
				++f->csr;
			}
			
			f_savecsr(f);
			
			unfolded = true;
		}
		
		if ((prevcp == '(' && curcp == ')')
			|| (prevcp == '[' && curcp == ']')
			|| (prevcp == '{' && curcp == '}'))
		{
			u32 csr = f->csr;
			f_writech(f, e_fromcodepoint('\n'), csr);
			++csr;
			
			for (u32 i = 0; i < ntab; ++i)
			{
				f_writech(f, e_fromcodepoint('\t'), csr);
				++csr;
			}
		}
	}
	
	if (unfolded)
	{
		return;
	}
	
	f_writech(f, e_fromcodepoint('\n'), f->csr);
	++f->csr;
	
	while (ntab)
	{
		f_writech(f, e_fromcodepoint('\t'), f->csr);
		++f->csr;
		--ntab;
	}
	
	f_savecsr(f);
}

static void
b_undo(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (!f->histlen)
	{
		showinfo("binds: nothing to undo");
		return;
	}
	
	f_undo(f);
}

static void
b_newframe(void)
{
	if (w_state.nframes >= O_MAXFILES)
	{
		showerr("binds: cannot open more than %u frames!", O_MAXFILES);
		return;
	}
	
	w_state.frames[w_state.nframes++] = f_create();
	w_state.curframe = w_state.nframes - 1;
}

static void
b_killframe(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (!(f->flags & F_UNSAVED))
	{
		w_destroyframe(w_state.curframe);
		return;
	}
	
	b_installconfirmprompt();
	p_beginstr("frame has unsaved changes, kill anyway? (y/n)");
	p_prompt.csr = -1;
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		i_readkey();
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_SUCCESS)
	{
		w_destroyframe(w_state.curframe);
	}
}

static void
b_save(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->src)
	{
		if (f->flags & F_UNSAVED)
		{
			f_save(f);
		}
		return;
	}
	
	b_installpathprompt();
	p_beginstr("save as: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (p_iswritable(k))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	char *path = p_getdatastr();
	
	for (usize i = 0; i < w_state.nframes; ++i)
	{
		if (!w_state.frames[i].src)
		{
			continue;
		}
		
		if (ispathsame(w_state.frames[i].src, path))
		{
			showerr("binds: cannot save multiple files under one name!");
			free(path);
			return;
		}
	}
	
	f->src = path;
	f_save(f);
}

static void
b_focus(void)
{
	f_frame_t tmp = w_state.frames[w_state.curframe];
	w_state.frames[w_state.curframe] = w_state.frames[0];
	w_state.frames[0] = tmp;
	w_state.curframe = 0;
}

static void
b_openfile(void)
{
	if (w_state.nframes >= O_MAXFILES)
	{
		showerr("binds: cannot open more than %u frames!", O_MAXFILES);
		return;
	}
	
	b_installpathprompt();
	p_beginstr("open file: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
        if (p_iswritable(k))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
    }
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	char *path = p_getdatastr();
	
	for (usize i = 0; i < w_state.nframes; ++i)
	{
		if (!w_state.frames[i].src)
		{
			continue;
		}
		
		if (ispathsame(w_state.frames[i].src, path))
		{
			// redirect user to already open file.
			w_state.curframe = i;
			free(path);
			return;
		}
	}
	
	f_frame_t new;
	if (f_fromfile(&new, path))
	{
		free(path);
		return;
	}
	free(path);
	
	w_state.frames[w_state.nframes] = new;
	w_state.curframe = w_state.nframes;
	++w_state.nframes;
}

static void
b_search(void)
{
	b_installprompt();
	p_beginstr("search literally: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (p_iswritable(k))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	usize len;
	e_char_t *data = p_getdata(&len);
	if (!len)
	{
		free(data);
		return;
	}
	
	for (usize i = f->csr + 1; i + len <= f->len; ++i)
	{
		for (usize j = 0; j < len; ++j)
		{
			if (data[j].codepoint != f->buf[i + j].codepoint)
			{
				goto nextchar;
			}
		}
		
		f->csr = i;
		f_savecsr(f);
		free(data);
		return;
	nextchar:;
	}
	
	showinfo("binds: didn't find search string");
	free(data);
}

static void
b_revsearch(void)
{
	b_installprompt();
	p_beginstr("reverse search literally: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (p_iswritable(k))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	usize len;
	e_char_t *data = p_getdata(&len);
	if (!len)
	{
		free(data);
		return;
	}
	
	for (isize i = f->csr; i >= (isize)len; --i)
	{
		for (isize j = 0; j < (isize)len; ++j)
		{
			if (data[j].codepoint != f->buf[i - len + j].codepoint)
			{
				goto nextchar;
			}
		}
		
		f->csr = i - len;
		f_savecsr(f);
		free(data);
		return;
	nextchar:;
	}
	
	showinfo("binds: didn't find search string");
	free(data);
}

static void
b_fleftparen(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	f_writech(f, e_fromcodepoint('('), f->csr);
	f_writech(f, e_fromcodepoint(')'), f->csr + 1);
	++f->csr;
	f_savecsr(f);
}

static void
b_fleftbracket(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	f_writech(f, e_fromcodepoint('['), f->csr);
	f_writech(f, e_fromcodepoint(']'), f->csr + 1);
	++f->csr;
	f_savecsr(f);
}

static void
b_fleftbrace(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	f_writech(f, e_fromcodepoint('{'), f->csr);
	f_writech(f, e_fromcodepoint('}'), f->csr + 1);
	++f->csr;
	f_savecsr(f);
}

static void
b_fdoublequote(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	f_writech(f, e_fromcodepoint('"'), f->csr);
	f_writech(f, e_fromcodepoint('"'), f->csr + 1);
	++f->csr;
	f_savecsr(f);
}

static void
b_pleftparen(void)
{
	p_writech(e_fromcodepoint('('), p_prompt.csr);
	p_writech(e_fromcodepoint(')'), p_prompt.csr + 1);
	++p_prompt.csr;
}

static void
b_pleftbracket(void)
{
	p_writech(e_fromcodepoint('['), p_prompt.csr);
	p_writech(e_fromcodepoint(']'), p_prompt.csr + 1);
	++p_prompt.csr;
}

static void
b_pleftbrace(void)
{
	p_writech(e_fromcodepoint('{'), p_prompt.csr);
	p_writech(e_fromcodepoint('}'), p_prompt.csr + 1);
	++p_prompt.csr;
}

static void
b_pdoublequote(void)
{
	p_writech(e_fromcodepoint('"'), p_prompt.csr);
	p_writech(e_fromcodepoint('"'), p_prompt.csr + 1);
	++p_prompt.csr;
}

static void
b_paste(void)
{
	if (!w_state.clipboard)
	{
		showerr("binds: clipboard is empty!");
		return;
	}
	
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	f_savecsr(f);
	while (f->csr < f->len && f->buf[f->csr].codepoint != '\n')
	{
		++f->csr;
	}
	
	f_writech(f, e_fromcodepoint('\n'), f->csr);
	++f->csr;
	f_write(f, w_state.clipboard, f->csr, w_state.clipboardlen);
	f_loadcsr(f);
}

static void
b_copyline(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	u32 begin = f->csr;
	while (begin && f->buf[begin - 1].codepoint != '\n')
	{
		--begin;
	}
	u32 end = f->csr;
	while (end < f->len && f->buf[end].codepoint != '\n')
	{
		++end;
	}
	
	w_state.clipboardlen = end - begin;
	w_state.clipboard = hreallocarray(w_state.clipboard, end - begin, sizeof(e_char_t));
	hmemcpy(w_state.clipboard, &f->buf[begin], sizeof(e_char_t) * (end - begin));
}

static void
b_cutline(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	u32 begin = f->csr;
	while (begin && f->buf[begin - 1].codepoint != '\n')
	{
		--begin;
	}
	u32 end = f->csr;
	while (end < f->len && f->buf[end].codepoint != '\n')
	{
		++end;
	}
	
	f_savecsr(f);
	
	w_state.clipboardlen = end - begin;
	w_state.clipboard = hreallocarray(w_state.clipboard, end - begin, sizeof(e_char_t));
	hmemcpy(w_state.clipboard, &f->buf[begin], sizeof(e_char_t) * (end - begin));
	f_erase(f, begin, end + (end < f->len));
	
	f->csr = begin;
	f_loadcsr(f);
}

static void
b_ncopyline(void)
{
	b_installnumberprompt();
	p_beginstr("copy lines: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (k.codepoint < 128 && isdigit(k.codepoint))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	char *linestr = p_getdatastr();
	u64 lines = strtoll(linestr, NULL, 10);
	free(linestr);
	
	if (!lines)
	{
		showinfo("binds: ignoring copy of zero lines");
		return;
	}
	
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	u32 begin = f->csr;
	while (begin && f->buf[begin - 1].codepoint != '\n')
	{
		--begin;
	}
	
	u32 end = begin;
	while (end < f->len)
	{
		lines -= f->buf[end].codepoint == '\n';
		if (!lines)
		{
			break;
		}
		++end;
	}
	
	w_state.clipboardlen = end - begin;
	w_state.clipboard = hreallocarray(w_state.clipboard, end - begin, sizeof(e_char_t));
	hmemcpy(w_state.clipboard, &f->buf[begin], sizeof(e_char_t) * (end - begin));
}

static void
b_ncutline(void)
{
	b_installnumberprompt();
	p_beginstr("cut lines: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (k.codepoint < 128 && isdigit(k.codepoint))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	char *linestr = p_getdatastr();
	u64 lines = strtoll(linestr, NULL, 10);
	free(linestr);
	
	if (!lines)
	{
		showinfo("binds: ignoring cut of zero lines");
		return;
	}
	
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	u32 begin = f->csr;
	while (begin && f->buf[begin - 1].codepoint != '\n')
	{
		--begin;
	}
	
	u32 end = begin;
	while (end < f->len)
	{
		lines -= f->buf[end].codepoint == '\n';
		if (!lines)
		{
			break;
		}
		++end;
	}
	
	w_state.clipboardlen = end - begin;
	w_state.clipboard = hreallocarray(w_state.clipboard, end - begin, sizeof(e_char_t));
	hmemcpy(w_state.clipboard, &f->buf[begin], sizeof(e_char_t) * (end - begin));
	f_erase(f, begin, end + (end < f->len));
	
	f->csr = begin;
	f_loadcsr(f);
}

static void
b_zoom(void)
{
	u32 x, y;
	u32 w, h;
	w_arrangeframe(w_state.curframe, &x, &y, &w, &h);
	
	w_state.frames[w_state.curframe].start = 0;
	f_compbounds(&w_state.frames[w_state.curframe], w, h / 2);
}

static void
b_goto(void)
{
	b_installnumberprompt();
	p_beginstr("goto line: ");
    while(p_prompt.rc == 0)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (k.codepoint < 128 && isdigit(k.codepoint))
		{
			p_writech(k, p_prompt.csr);
			p_prompt.csr += p_prompt.csr < O_MAXPROMPTLEN;
		}
	}
	p_end();
	b_installbase();
	
	if (p_prompt.rc == P_FAIL)
	{
		return;
	}
	
	char *linestr = p_getdatastr();
	u64 line = strtoll(linestr, NULL, 10);
	free(linestr);
	line -= line > 0;
	
	f_frame_t *f = &w_state.frames[w_state.curframe];
	
	// move cursor to needed line.
	f->csr = 0;
	while (f->csr < f->len && line)
	{
		if (f->buf[f->csr++].codepoint == '\n')
		{
			--line;
		}
	}
	f_savecsr(f);
	
	// focus selected line.
	u32 x, y;
	u32 w, h;
	w_arrangeframe(w_state.curframe, &x, &y, &w, &h);
	
	f->start = 0;
	f_compbounds(f, w, h / 2);
}

static void
b_recmacro(void)
{
	if (!i_isrecmacro())
	{
		showinfo("binds: recording macro");
		i_recmacro();
	}
}

static void
b_execmacro(void)
{
	if (i_isrecmacro())
	{
		showinfo("binds: stopped recording macro");
		i_stoprecmacro();
	}
	else
	{
		showinfo("binds: executing macro");
		i_execmacro();
	}
}
