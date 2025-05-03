// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct w_wndstate
{
	f_frame frames[O_MAXFILES];
	usize curframe, nframes;
	e_char *clipboard;
	u32 clipboardlen;
	bool running;
	bool writeinput;
} w_wndstate;

extern w_wndstate w_state;

i32 w_init(void);
void w_loop(void);
void w_render(void);
