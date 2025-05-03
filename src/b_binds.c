// SPDX-License-Identifier: GPL-3.0-or-later

static void b_mvleft(void);
static void b_mvright(void);
static void b_mvup(void);
static void b_mvdown(void);
static void b_mvstart(void);
static void b_mvend(void);
static void b_quit(void);

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
	i_organize();
}

static void
b_mvleft(void)
{
	f_frame *f = &w_state.frames[w_state.curframe];
	if (f->csr > 0)
	{
		--f->csr;
		f_savecsr(f);
	}
}

static void
b_mvright(void)
{
	f_frame *f = &w_state.frames[w_state.curframe];
	if (f->csr < f->len)
	{
		++f->csr;
		f_savecsr(f);
	}
}

static void
b_mvup(void)
{
	f_frame *f = &w_state.frames[w_state.curframe];
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
	f_frame *f = &w_state.frames[w_state.curframe];
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
	f_frame *f = &w_state.frames[w_state.curframe];
	while (f->csr > 0 && f->buf[f->csr - 1].codepoint != '\n')
	{
		--f->csr;
	}
	f_savecsr(f);
}

static void
b_mvend(void)
{
	f_frame *f = &w_state.frames[w_state.curframe];
	while (f->csr < f->len && f->buf[f->csr].codepoint != '\n')
	{
		++f->csr;
	}
	f_savecsr(f);
}

static void
b_quit(void)
{
	w_state.running = false;
}
