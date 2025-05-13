// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct w_state
{
	f_frame_t frames[O_MAXFILES];
	usize curframe, nframes;
	e_char_t *clipboard;
	u32 clipboardlen;
	bool running;
	bool writeinput;
} w_state_t;

extern w_state_t w_state;

i32 w_init(void);
void w_loop(void);
void w_arrangeframe(usize idx, OUT u32 *x, OUT u32 *y, OUT u32 *w, OUT u32 *h);
void w_render(void);
bool w_iswritable(e_char_t ch);
void w_destroyframe(usize idx);
void w_zoom(void);
