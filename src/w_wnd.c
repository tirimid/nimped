// SPDX-License-Identifier: GPL-3.0-or-later

w_wndstate w_state;

static bool w_iswritable(e_char ch);

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
		
		e_char k = i_readkey();
		if (w_state.writeinput && w_iswritable(k))
		{
			f_frame *f = &w_state.frames[w_state.curframe];
			f_writech(f, k, f->csr);
			++f->csr;
		}
	}
}

void
w_render(void)
{
	u32 rw, rh;
	r_winsize(&rw, &rh);
	
	r_clear(e_fromcodepoint(' '), (r_attr){o_opts.globalfg, o_opts.globalbg});
	
	// mono frame.
	if (w_state.nframes == 1)
	{
		f_compbounds(&w_state.frames[0], rw, rh);
		f_render(&w_state.frames[0], 0, 0, rw, rh, !w_state.curframe);
		return;
	}
	
	// master frame.
	f_compbounds(&w_state.frames[0], o_opts.masternum * rw / o_opts.masterdenom, rh);
	f_render(
		&w_state.frames[0],
		0,
		0,
		o_opts.masternum * rw / o_opts.masterdenom,
		rh,
		!w_state.curframe
	);
	
	// secondary frames.
	for (usize i = 1; i < w_state.nframes; ++i)
	{
		u32 h = rh / (w_state.nframes - 1);
		u32 y = (i - 1) * h;
		u32 x = o_opts.masternum * rw / o_opts.masterdenom;
		u32 w = rw - x;
		
		if (y + h > rh || i == w_state.nframes - 1)
		{
			h = rh - y;
		}
		
		f_compbounds(&w_state.frames[i], w, h);
		f_render(&w_state.frames[i], x, y, w, h, i == w_state.curframe);
	}
}

static bool
w_iswritable(e_char ch)
{
	// input returns replacement unicode character on non-writing key, and it is
	// rare that a user wants to input a replacement character, so they are all
	// just considered non-writable.
	return (ch.codepoint == '\t' || e_isprint(ch)) && ch.codepoint != E_REPLACEMENT;
}
