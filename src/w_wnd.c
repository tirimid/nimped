// SPDX-License-Identifier: GPL-3.0-or-later

w_wndstate w_state;

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
		if (k.codepoint == E_INVALIDCODEPOINT)
		{
			continue;
		}
		
		// TODO: handle key input.
	}
}

void
w_render(void)
{
	r_clear(e_fromcodepoint(' '), (r_attr){o_opts.globalfg, o_opts.globalbg});
	
	u32 rw, rh;
	r_winsize(&rw, &rh);
	
	if (w_state.nframes == 1)
	{
		f_render(&w_state.frames[0], 0, 0, rw, rh, !w_state.curframe);
		return;
	}
	
	f_render(
		&w_state.frames[0],
		0,
		0,
		o_opts.masternum * rw / o_opts.masterdenom,
		rh,
		!w_state.curframe
	);
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
		
		f_render(&w_state.frames[i], x, y, w, h, i == w_state.curframe);
	}
}
