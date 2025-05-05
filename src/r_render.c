// SPDX-License-Identifier: GPL-3.0-or-later

static void r_sigwinch(int arg);

static e_char_t *r_cellchars;
static r_attr_t *r_cellattrs;
static u32 r_w, r_h;
static struct termios r_oldtermios;

i32
r_init(void)
{
	if (tcgetattr(STDIN_FILENO, &r_oldtermios))
	{
		showerr("render: failed on tcgetattr() for old stdin!");
		return 1;
	}
	
	struct termios new = r_oldtermios;
	new.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	new.c_iflag &= ~(ICRNL | IXON | ISTRIP);
	new.c_oflag &= ~OPOST;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new))
	{
		showerr("render failed on tcsetattr() for new stdin!");
		return 1;
	}
	
	setvbuf(stdin, NULL, _IONBF, 0);
	
	printf("\x1b[?25l");
	
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	r_w = ws.ws_col;
	r_h = ws.ws_row;
	
	r_cellchars = calloc(r_w * r_h, sizeof(e_char_t));
	r_cellattrs = calloc(r_w * r_h, sizeof(r_attr_t));
	
	struct sigaction sa;
	sigaction(SIGWINCH, NULL, &sa);
	sa.sa_handler = r_sigwinch;
	sigaction(SIGWINCH, &sa, NULL);
	
	return 0;
}

void
r_quit(bool clearscr)
{
	printf("\x1b[?25h\x1b[0m\r");
	
	if (clearscr)
	{
		printf("\x1b[2J\x1b[H");
	}
	
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &r_oldtermios))
	{
		showerr("render: failed on tcsetattr() for old stdin!");
		return;
	}
}

void
r_clear(e_char_t ch, r_attr_t a)
{
	for (usize i = 0; i < r_w * r_h; ++i)
	{
		r_cellchars[i] = ch;
		r_cellattrs[i] = a;
	}
}

void
r_fillch(e_char_t ch, u32 x, u32 y, u32 w, u32 h)
{
	if (x >= r_w || y >= r_h)
	{
		return;
	}
	
	w = x + w >= r_w ? r_w - x : w;
	h = y + h >= r_h ? r_h - y : h;
	
	for (u32 cx = x; cx < x + w; ++cx)
	{
		for (u32 cy = y; cy < y + h; ++cy)
		{
			r_cellchars[r_w * cy + cx] = ch;
		}
	}
}

void
r_fillattr(r_attr_t a, u32 x, u32 y, u32 w, u32 h)
{
	if (x >= r_w || y >= r_h)
	{
		return;
	}
	
	w = x + w >= r_w ? r_w - x : w;
	h = y + h >= r_h ? r_h - y : h;
	
	for (u32 cx = x; cx < x + w; ++cx)
	{
		for (u32 cy = y; cy < y + h; ++cy)
		{
			r_cellattrs[r_w * cy + cx] = a;
		}
	}
}

void
r_fill(e_char_t ch, r_attr_t a, u32 x, u32 y, u32 w, u32 h)
{
	if (x >= r_w || y >= r_h)
	{
		return;
	}
	
	w = x + w >= r_w ? r_w - x : w;
	h = y + h >= r_h ? r_h - y : h;
	
	for (u32 cx = x; cx < x + w; ++cx)
	{
		for (u32 cy = y; cy < y + h; ++cy)
		{
			r_cellchars[r_w * cy + cx] = ch;
			r_cellattrs[r_w * cy + cx] = a;
		}
	}
}

void
r_putch(e_char_t ch, u32 x, u32 y)
{
	if (x >= r_w || y >= r_h)
	{
		return;
	}
	
	r_cellchars[r_w * y + x] = ch;
}

void
r_putattr(r_attr_t a, u32 x, u32 y)
{
	if (x >= r_w || y >= r_h)
	{
		return;
	}
	
	r_cellattrs[r_w * y + x] = a;
}

void
r_put(e_char_t ch, r_attr_t a, u32 x, u32 y)
{
	if (x >= r_w || y >= r_h)
	{
		return;
	}
	
	r_cellchars[r_w * y + x] = ch;
	r_cellattrs[r_w * y + x] = a;
}

void
r_present(void)
{
	r_attr_t a = {0};
	printf("\x1b[38;5;0m\x1b[48;5;0m");
	
	for (usize i = 0; i < r_w * r_h; ++i)
	{
		if (r_cellattrs[i].fg != a.fg || r_cellattrs[i].bg != a.bg)
		{
			a = r_cellattrs[i];
			printf("\x1b[38;5;%um\x1b[48;5;%um", a.fg, a.bg);
		}
		
		e_putch(r_cellchars[i]);
	}
}

void
r_winsize(OUT u32 *w, OUT u32 *h)
{
	if (w)
	{
		*w = r_w;
	}
	
	if (h)
	{
		*h = r_h;
	}
}

static void
r_sigwinch(int arg)
{
	(void)arg;
	
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	r_w = ws.ws_col;
	r_h = ws.ws_row;
	
	r_cellchars = reallocarray(r_cellchars, r_w * r_h, sizeof(e_char_t));
	r_cellattrs = reallocarray(r_cellattrs, r_w * r_h, sizeof(r_attr_t));
	
	printf("\r");
}
