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
	i_organize();
	
	w_state.writeinput = true;
	
	r_setbarstr(O_WRITENAME);
}

void
b_installpathprompt(void)
{
	i_unbind();
	i_bind(o_bquit, b_quitpromptfail);
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
	i_organize();
	
	w_state.writeinput = false;
}

void
b_installconfirmprompt(void)
{
	i_unbind();
	i_bind(o_bquit, b_quitpromptfail);
	i_bind(o_bnewline, b_quitpromptsuccess);
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
	while (f->csr && !e_isalpha(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	while (f->csr && e_isalpha(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	f_savecsr(f);
}

static void
b_fmvwordright(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr < f->len && !e_isalpha(f->buf[f->csr]))
	{
		++f->csr;
	}
	while (f->csr < f->len && e_isalpha(f->buf[f->csr]))
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
		&& !e_isalpha(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
	while ((u32)p_prompt.csr > p_prompt.start
		&& e_isalpha(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
}

static void
b_pmvwordright(void)
{
	while ((u32)p_prompt.csr < p_prompt.len
		&& !e_isalpha(p_prompt.data[p_prompt.csr]))
	{
		++p_prompt.csr;
	}
	while ((u32)p_prompt.csr < p_prompt.len
		&& e_isalpha(p_prompt.data[p_prompt.csr]))
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
		p_beginstr("frames have unsaved changes, quit anyway?");
		p_prompt.csr = -1;
		while (!p_prompt.rc)
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
	
	// TODO: add support for smart paren deletion.
	
	--f->csr;
	f_erase(f, f->csr, f->csr + 1);
	f_savecsr(f);
}

static void
b_fdelword(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	u32 ub = f->csr;
	while (f->csr && !e_isalpha(f->buf[f->csr - 1]))
	{
		--f->csr;
	}
	while (f->csr && e_isalpha(f->buf[f->csr - 1]))
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
	
	// TODO: add support for smart paren deletion.
	
	--p_prompt.csr;
	p_erase(p_prompt.csr, p_prompt.csr + 1);
}

static void
b_pdelword(void)
{
	u32 ub = p_prompt.csr;
	while ((u32)p_prompt.csr > p_prompt.start
		&& !e_isalpha(p_prompt.data[p_prompt.csr - 1]))
	{
		--p_prompt.csr;
	}
	while ((u32)p_prompt.csr > p_prompt.start
		&& e_isalpha(p_prompt.data[p_prompt.csr - 1]))
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
	// TODO: add support for features like unfolding smart parens.
	f_frame_t *f = &w_state.frames[w_state.curframe];
	f_writech(f, e_fromcodepoint('\n'), f->csr);
	++f->csr;
	f_savecsr(f);
}

static void
b_undo(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (!f->histlen)
	{
		showerr("binds: nothing to undo!");
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
	p_beginstr("frame has unsaved changes, kill anyway?");
	p_prompt.csr = -1;
	while (!p_prompt.rc)
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
	// TODO: implement.
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
	while (!p_prompt.rc)
	{
		w_render();
		p_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (w_iswritable(k))
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
