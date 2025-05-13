// SPDX-License-Identifier: GPL-3.0-or-later

static void r_sigwinch(int arg);

static e_char_t *r_cellchars;
static r_attr_t *r_cellattrs;
static u32 r_w, r_h;
static struct termios r_oldtermios;
static e_char_t r_bar[O_MAXBARLEN];
static usize r_barlen;
static u32 r_barh = 1;

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

e_char_t
r_getch(u32 x, u32 y)
{
	return r_cellchars[r_w * y + x];
}

r_attr_t
r_getattr(u32 x, u32 y)
{
	return r_cellattrs[r_w * y + x];
}

void
r_get(OUT e_char_t *ch, OUT r_attr_t *a, u32 x, u32 y)
{
	*ch = r_cellchars[r_w * y + x];
	*a = r_cellattrs[r_w * y + x];
}

void
r_present(void)
{
	// allow cursor to be drawn on newline if width is exceeded.
	u32 barh = r_barh + ((u32)p_prompt.csr >= p_prompt.len && p_prompt.csr % r_w == 0);
	
	r_attr_t a = {0};
	printf("\r\x1b[38;5;0m\x1b[48;5;0m");
	
	for (usize i = 0; i < r_w * (r_h - barh); ++i)
	{
		if (r_cellattrs[i].fg != a.fg || r_cellattrs[i].bg != a.bg)
		{
			a = r_cellattrs[i];
			printf("\x1b[38;5;%um\x1b[48;5;%um", a.fg, a.bg);
		}
		
		e_putch(r_cellchars[i]);
	}
	
	printf("\x1b[38;5;%um\x1b[48;5;%um", o_opts.globalfg, o_opts.globalbg);
	for (usize i = 0; i < r_w * barh; ++i)
	{
		if ((i64)i == p_prompt.csr)
		{
			printf("\x1b[38;5;%um\x1b[48;5;%um", o_opts.csrfg, o_opts.csrbg);
		}
		
		if (i < r_barlen)
		{
			e_putch(r_bar[i]);
		}
		else
		{
			printf(" ");
		}
		
		if ((i64)i == p_prompt.csr)
		{
			printf("\x1b[38;5;%um\x1b[48;5;%um", o_opts.globalfg, o_opts.globalbg);
		}
	}
}

void
r_winsize(OUT u32 *w, OUT u32 *h)
{
	*w = r_w;
	*h = r_h - r_barh - ((u32)p_prompt.csr >= p_prompt.len && p_prompt.csr % r_w == 0);
}

void
r_setbar(e_char_t const *s, usize n)
{
	r_barlen = n > O_MAXBARLEN ? O_MAXBARLEN : n;
	
	memcpy(r_bar, s, sizeof(e_char_t) * r_barlen);
	
	r_barh = (r_barlen + r_w - 1) / r_w;
	r_barh += !r_barh;
}

void
r_setbarstr(char const *s)
{
	usize n = strlen(s);
	r_barlen = n > O_MAXBARLEN ? O_MAXBARLEN : n;
	
	for (usize i = 0; i < r_barlen;)
	{
		usize nbytes = e_frommem(&r_bar[i], (u8 const *)&s[i]);
		i += nbytes ? nbytes : 1;
	}
	
	r_barh = (r_barlen + r_w - 1) / r_w;
	r_barh += !r_barh;
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
	
	r_setbar(r_bar, r_barlen); // recompute bar.
}
