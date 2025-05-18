// SPDX-License-Identifier: GPL-3.0-or-later

w_state_t w_state;

i32
w_init(void)
{
	for (usize i = 0; i < a_args.nfiles; ++i)
	{
		if (f_fromfile(&w_state.frames[i], a_args.files[i]))
		{
			return 1;
		}
		++w_state.nframes;
	}
	
	if (!w_state.nframes)
	{
		w_state.frames[0] = f_fromstr(O_GREETERTEXT);
		++w_state.nframes;
	}
	
	b_installbase();
	
	return 0;
}

void
w_loop(void)
{
	w_state.running = true;
	while (w_state.running)
	{
		w_render();
		r_present();
		
		e_char_t k = i_readkey();
		if (w_state.writeinput && w_iswritable(k))
		{
			f_frame_t *f = &w_state.frames[w_state.curframe];
			f_writech(f, k, f->csr);
			++f->csr;
			f_savecsr(f);
		}
	}
}

void
w_arrangeframe(usize idx, OUT u32 *x, OUT u32 *y, OUT u32 *w, OUT u32 *h)
{
	u32 rw, rh;
	r_winsize(&rw, &rh);
	
	if (w_state.nframes == 1)
	{
		*x = 0;
		*y = 0;
		*w = rw;
		*h = rh;
		return;
	}
	
	if (!idx)
	{
		*x = 0;
		*y = 0;
		*w = o_opts.masternum * rw / o_opts.masterdenom;
		*h = rh;
	}
	else
	{
		*h = rh / (w_state.nframes - 1);
		*y = (idx - 1) * *h;
		*x = o_opts.masternum * rw / o_opts.masterdenom;
		*w = rw - *x;
		
		if (*y + *h > rh || idx == w_state.nframes - 1)
		{
			*h = rh - *y;
		}
	}
}

void
w_render(void)
{
	for (usize i = 0; i < w_state.nframes; ++i)
	{
		u32 x, y;
		u32 w, h;
		w_arrangeframe(i, &x, &y, &w, &h);
		
		f_compbounds(&w_state.frames[i], w, h);
		f_render(&w_state.frames[i], x, y, w, h, i == w_state.curframe);
	}
}

bool
w_iswritable(e_char_t ch)
{
	// input returns replacement unicode character on non-writing key, and it is
	// rare that a user wants to input a replacement character, so they are all
	// just considered non-writable.
	return (ch.codepoint == '\t' || e_isprint(ch)) && ch.codepoint != E_REPLACEMENT;
}

void
w_destroyframe(usize idx)
{
	f_destroy(&w_state.frames[idx]);
	
	if (w_state.nframes == 1)
	{
		w_state.frames[0] = f_create();
		return;
	}
	
	hmemmove(
		&w_state.frames[idx],
		&w_state.frames[idx + 1],
		sizeof(f_frame_t) * (w_state.nframes - idx)
	);
	--w_state.nframes;
	
	if (w_state.curframe && w_state.curframe >= idx)
	{
		--w_state.curframe;
	}
}
