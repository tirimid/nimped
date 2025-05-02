// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct r_attr
{
	u8 fg, bg;
} r_attr;

i32 r_init(void);
void r_quit(void);
void r_clear(e_char ch, r_attr a);
void r_fill(e_char ch, r_attr a, u32 x, u32 y, u32 w, u32 h);
void r_putch(e_char ch, u32 x, u32 y);
void r_putattr(r_attr a, u32 x, u32 y);
void r_put(e_char ch, r_attr a, u32 x, u32 y);
void r_present(void);
void r_winsize(OUT u32 *w, OUT u32 *h);
