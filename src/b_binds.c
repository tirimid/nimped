// SPDX-License-Identifier: GPL-3.0-or-later

static void b_mvleft(void);
static void b_mvright(void);
static void b_mvup(void);
static void b_mvdown(void);
static void b_mvstart(void);
static void b_mvend(void);
static void b_quit(void);
static void b_next(void);
static void b_prev(void);
static void b_delfront(void);
static void b_delback(void);
static void b_delword(void);
static void b_newline(void);
static void b_undo(void);
static void b_newframe(void);
static void b_killframe(void);
static void b_save(void);
static void b_focus(void);

void
b_installbase(void)
{
	i_unbind();
	i_bind(o_bmvleft, b_mvleft);
	i_bind(o_bmvright, b_mvright);
	i_bind(o_bmvup, b_mvup);
	i_bind(o_bmvdown, b_mvdown);
	i_bind(o_bmvstart, b_mvstart);
	i_bind(o_bmvend, b_mvend);
	i_bind(o_bquit, b_quit);
	i_bind(o_bnext, b_next);
	i_bind(o_bprev, b_prev);
	i_bind(o_bwritemode, b_installwrite);
	i_bind(o_bundo, b_undo);
	i_bind(o_bnewframe, b_newframe);
	i_bind(o_bkillframe, b_killframe);
	i_bind(o_bsave, b_save);
	i_bind(o_bfocus, b_focus);
	i_organize();
	
	w_state.writeinput = false;
	
	r_setbarstr(O_BASENAME);
}

void
b_installwrite(void)
{
	i_unbind();
	i_bind(o_bquit, b_installbase);
	i_bind(o_bdelfront, b_delfront);
	i_bind(o_bdelback, b_delback);
	i_bind(o_bdelword, b_delword);
	i_bind(o_bnewline, b_newline);
	i_organize();
	
	w_state.writeinput = true;
	
	r_setbarstr(O_WRITENAME);
}

static void
b_mvleft(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->csr > 0)
	{
		--f->csr;
		f_savecsr(f);
	}
}

static void
b_mvright(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->csr < f->len)
	{
		++f->csr;
		f_savecsr(f);
	}
}

static void
b_mvup(void)
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
b_mvdown(void)
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
b_mvstart(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr > 0 && f->buf[f->csr - 1].codepoint != '\n')
	{
		--f->csr;
	}
	f_savecsr(f);
}

static void
b_mvend(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	while (f->csr < f->len && f->buf[f->csr].codepoint != '\n')
	{
		++f->csr;
	}
	f_savecsr(f);
}

static void
b_quit(void)
{
	// TODO: implement checks for unsaved frames.
	w_state.running = false;
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
b_delfront(void)
{
	f_frame_t *f = &w_state.frames[w_state.curframe];
	if (f->csr < f->len)
	{
		f_erase(f, f->csr, f->csr + 1);
	}
}

static void
b_delback(void)
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
b_delword(void)
{
	// TODO: implement.
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
	// TODO: implement.
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
