// SPDX-License-Identifier: GPL-3.0-or-later

#define H_NUMBERINIT "0123456789"
#define H_NUMBER "xob+-.0123456789aAbBcCdDeEfF_"
#define H_CSPECIAL "!%&()*+,-./:;<=>?[\\]^{|}~"
#define H_CWORDINIT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define H_CWORD "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
#define H_SHSPECIAL "$\\=[]!><|;{}()*?~&`:."
#define H_SHWORDINIT H_CWORDINIT
#define H_SHWORD H_CWORD
#define H_JSSPECIAL "()[]{};=+-*/%<>&^|?!~:,."
#define H_JSWORDINIT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$"
#define H_JSWORD "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$0123456789"
#define H_CCSPECIAL "!%&()*+,-./:;<=>?[\\]^{|}~"
#define H_CCWORDINIT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define H_CCWORD "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"

static bool h_cmpstr(f_frame_t const *f, char const *cmp, u32 at);
static bool h_cmpany(f_frame_t const *f, char const *cmp, u32 at);
static void h_linecomment(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_number(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_string(OUT h_region_t *r, f_frame_t const *f, u32 from, bool escape, bool newline);
static void h_special(OUT h_region_t *r, f_frame_t const *f, u32 from, char const *chars);
static bool h_trykeyword(OUT h_region_t *r, f_frame_t const *f, u32 from, u32 end, o_langmode_t lang);
static void h_cpreproc(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_ccomment(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_cword(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_shword(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_jsword(OUT h_region_t *r, f_frame_t const *f, u32 from);
static void h_ccword(OUT h_region_t *r, f_frame_t const *f, u32 from);

void
h_find(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	if (!f->src)
	{
		*r = (h_region_t)
		{
			.lb = f->len,
			.ub = f->len
		};
		return;
	}
	
	char const *ext = fileext(f->src);
	if (!strcmp(ext, "c") || !strcmp(ext, "h"))
	{
		h_findc(r, f, from);
	}
	else if (!strcmp(ext, "cc") || !strcmp(ext, "hh"))
	{
		h_findcc(r, f, from);
	}
	else if (!strcmp(ext, "sh"))
	{
		h_findsh(r, f, from);
	}
	else if (!strcmp(ext, "js"))
	{
		h_findjs(r, f, from);
	}
	else
	{
		*r = (h_region_t)
		{
			.lb = f->len,
			.ub = f->len
		};
	}
}

void
h_findc(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	for (u32 i = from; i < f->len; ++i)
	{
		if (h_cmpstr(f, "//", i))
		{
			h_linecomment(r, f, i);
			return;
		}
		else if (h_cmpstr(f, "/*", i))
		{
			h_ccomment(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_NUMBERINIT, i))
		{
			h_number(r, f, i);
			return;
		}
		else if (h_cmpstr(f, "#", i))
		{
			h_cpreproc(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_CSPECIAL, i))
		{
			h_special(r, f, i, H_CSPECIAL);
			return;
		}
		else if (h_cmpany(f, H_CWORDINIT, i))
		{
			h_cword(r, f, i);
			return;
		}
		else if (h_cmpany(f, "\"'", i))
		{
			h_string(r, f, i, true, false);
			return;
		}
	}
	
	*r = (h_region_t)
	{
		.lb = f->len,
		.ub = f->len
	};
}

void
h_findsh(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	for (u32 i = from; i < f->len; ++i)
	{
		if (h_cmpstr(f, "#", i))
		{
			h_linecomment(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_NUMBERINIT, i))
		{
			h_number(r, f, i);
			return;
		}
		else if (h_cmpstr(f, "'", i))
		{
			h_string(r, f, i, false, true);
			return;
		}
		else if (h_cmpstr(f, "\"", i))
		{
			h_string(r, f, i, true, true);
			return;
		}
		else if (h_cmpany(f, H_SHSPECIAL, i))
		{
			h_special(r, f, i, H_SHSPECIAL);
			return;
		}
		else if (h_cmpany(f, H_SHWORDINIT, i))
		{
			h_shword(r, f, i);
			return;
		}
	}
	
	*r = (h_region_t)
	{
		.lb = f->len,
		.ub = f->len
	};
}

void
h_findjs(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	for (u32 i = from; i < f->len; ++i)
	{
		// regex highlight will not be implemented.
		
		if (h_cmpstr(f, "//", i) || h_cmpstr(f, "#!", i))
		{
			h_linecomment(r, f, i);
			return;
		}
		else if (h_cmpstr(f, "/*", i))
		{
			h_ccomment(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_NUMBERINIT, i))
		{
			h_number(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_JSWORDINIT, i))
		{
			h_jsword(r, f, i);
			return;
		}
		else if (h_cmpany(f, "\"'`", i))
		{
			h_string(r, f, i, true, false);
			return;
		}
		else if (h_cmpany(f, H_JSSPECIAL, i))
		{
			h_special(r, f, i, H_JSSPECIAL);
			return;
		}
	}
	
	*r = (h_region_t)
	{
		.lb = f->len,
		.ub = f->len
	};
}

void
h_findcc(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	for (u32 i = from; i < f->len; ++i)
	{
		if (h_cmpstr(f, "//", i))
		{
			h_linecomment(r, f, i);
			return;
		}
		else if (h_cmpstr(f, "/*", i))
		{
			h_ccomment(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_NUMBERINIT, i))
		{
			h_number(r, f, i);
			return;
		}
		else if (h_cmpstr(f, "#", i))
		{
			h_cpreproc(r, f, i);
			return;
		}
		else if (h_cmpany(f, H_CCSPECIAL, i))
		{
			h_special(r, f, i, H_CCSPECIAL);
			return;
		}
		else if (h_cmpany(f, H_CCWORDINIT, i))
		{
			h_ccword(r, f, i);
			return;
		}
		else if (h_cmpany(f, "\"'", i))
		{
			h_string(r, f, i, true, false);
			return;
		}
	}
	
	*r = (h_region_t)
	{
		.lb = f->len,
		.ub = f->len
	};
}

static bool
h_cmpstr(f_frame_t const *f, char const *cmp, u32 at)
{
	for (; *cmp && at < f->len; ++at, ++cmp)
	{
		if ((u32)*cmp != f->buf[at].codepoint)
		{
			return false;
		}
	}
	return !*cmp;
}

static bool
h_cmpany(f_frame_t const *f, char const *cmp, u32 at)
{
	for (; *cmp; ++cmp)
	{
		if ((u32)*cmp == f->buf[at].codepoint)
		{
			return true;
		}
	}
	return false;
}

static void
h_linecomment(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	u32 end = from;
	while (end < f->len && f->buf[end].codepoint != '\n')
	{
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end,
		.fg = o_opts.commentfg,
		.bg = o_opts.commentbg
	};
}

static void
h_number(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	u32 end = from;
	while (end < f->len && strchr(H_NUMBER, f->buf[end].codepoint))
	{
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end,
		.fg = o_opts.numberfg,
		.bg = o_opts.numberbg
	};
}

static void
h_string(
	OUT h_region_t *r,
	f_frame_t const *f,
	u32 from,
	bool escape,
	bool newline
)
{
	e_char_t quote = f->buf[from];
	
	u32 end = from + 1;
	while (end < f->len)
	{
		if (!newline && f->buf[end].codepoint == '\n')
		{
			break;
		}
		
		if (f->buf[end].codepoint == quote.codepoint)
		{
			++end;
			break;
		}
		
		end += (escape && f->buf[end].codepoint == '\\') + 1;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end,
		.fg = o_opts.stringfg,
		.bg = o_opts.stringbg
	};
}

static void
h_special(OUT h_region_t *r, f_frame_t const *f, u32 from, char const *chars)
{
	u32 end = from;
	while (end < f->len && strchr(chars, f->buf[end].codepoint))
	{
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end,
		.fg = o_opts.specialfg,
		.bg = o_opts.specialbg
	};
}

static bool
h_trykeyword(
	OUT h_region_t *r,
	f_frame_t const *f,
	u32 from,
	u32 end,
	o_langmode_t lang
)
{
	for (usize i = 0; i < o_opts.lang[lang].nkeywords; ++i)
	{
		u16 kwlen = o_opts.lang[lang].keywordlen[i];
		if (end - from != kwlen)
		{
			continue;
		}
		
		e_char_t const *kw = o_opts.lang[lang].keywords[i];
		if (!memcmp(kw, &f->buf[from], sizeof(e_char_t) * kwlen))
		{
			r->fg = o_opts.keywordfg;
			r->bg = o_opts.keywordbg;
			return true;
		}
	}
	
	for (usize i = 0; i < o_opts.lang[lang].nprimitives; ++i)
	{
		u16 primlen = o_opts.lang[lang].primitivelen[i];
		if (end - from != primlen)
		{
			continue;
		}
		
		e_char_t const *prim = o_opts.lang[lang].primitives[i];
		if (!memcmp(prim, &f->buf[from], sizeof(e_char_t) * primlen))
		{
			r->fg = o_opts.primitivefg;
			r->bg = o_opts.primitivebg;
			return true;
		}
	}
	
	return false;
}

static void
h_cpreproc(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	u32 end = from;
	bool cont = false;
	while (end < f->len && (cont || f->buf[end].codepoint != '\n'))
	{
		if (!e_isspace(f->buf[end]))
		{
			cont = f->buf[end].codepoint == '\\';
		}
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end,
		.fg = o_opts.macrofg,
		.bg = o_opts.macrobg
	};
}

static void
h_ccomment(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	u32 end = from + 2;
	while (end < f->len && !h_cmpstr(f, "*/", end))
	{
		++end;
	}
	end += 2 * (end < f->len);
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end,
		.fg = o_opts.commentfg,
		.bg = o_opts.commentbg
	};
}

static void
h_cword(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	i32 nlower = 0;
	u32 end = from;
	while (end < f->len && strchr(H_CWORD, f->buf[end].codepoint))
	{
		nlower += islower(f->buf[end].codepoint);
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end
	};
	
	if (h_trykeyword(r, f, from, end, O_CMODE))
	{
		return;
	}
	
	if (end - from >= 2
		&& f->buf[end - 1].codepoint == 't'
		&& f->buf[end - 2].codepoint == '_')
	{
		r->fg = o_opts.typefg;
		r->bg = o_opts.typebg;
		return;
	}
	
	if (!nlower)
	{
		r->fg = o_opts.macrofg;
		r->bg = o_opts.macrobg;
		return;
	}
	
	u32 next = end;
	while (next < f->len && e_isspace(f->buf[next]))
	{
		++next;
	}
	
	if (next < f->len && f->buf[next].codepoint == '(')
	{
		r->fg = o_opts.emphfg;
		r->bg = o_opts.emphbg;
		return;
	}
	
	r->fg = o_opts.normfg;
	r->bg = o_opts.normbg;
}

static void
h_shword(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	i32 nlower = 0;
	u32 end = from;
	while (end < f->len && strchr(H_CWORD, f->buf[end].codepoint))
	{
		nlower += islower(f->buf[end].codepoint);
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end
	};
	
	if (h_trykeyword(r, f, from, end, O_SHMODE))
	{
		return;
	}
	
	if (!nlower)
	{
		r->fg = o_opts.emphfg;
		r->bg = o_opts.emphbg;
		return;
	}
	
	r->fg = o_opts.normfg;
	r->bg = o_opts.normbg;
}

static void
h_jsword(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	u32 end = from;
	while (end < f->len && strchr(H_JSWORD, f->buf[end].codepoint))
	{
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end
	};
	
	if (h_trykeyword(r, f, from, end, O_JSMODE))
	{
		return;
	}
	
	if (isupper(f->buf[from].codepoint))
	{
		r->fg = o_opts.emphfg;
		r->bg = o_opts.emphbg;
		return;
	}
	
	r->fg = o_opts.normfg;
	r->bg = o_opts.normbg;
}

static void
h_ccword(OUT h_region_t *r, f_frame_t const *f, u32 from)
{
	i32 nlower = 0;
	u32 end = from;
	while (end < f->len && strchr(H_CCWORD, f->buf[end].codepoint))
	{
		nlower += islower(f->buf[end].codepoint);
		++end;
	}
	
	*r = (h_region_t)
	{
		.lb = from,
		.ub = end
	};
	
	if (h_trykeyword(r, f, from, end, O_CCMODE))
	{
		return;
	}
	
	if (end - from >= 2
		&& f->buf[end - 1].codepoint == 't'
		&& f->buf[end - 2].codepoint == '_')
	{
		r->fg = o_opts.typefg;
		r->bg = o_opts.typebg;
		return;
	}
	
	if (!nlower)
	{
		r->fg = o_opts.macrofg;
		r->bg = o_opts.macrobg;
		return;
	}
	
	u32 next = end;
	while (next < f->len && e_isspace(f->buf[next]))
	{
		++next;
	}
	
	// very dumb way of doing checks for template parameters; only works for "well
	// behaved" / "well formed" code, but honestly should be good enough.
	if (next < f->len && f->buf[next].codepoint == '<')
	{
		++next;
		for (u32 nopen = 1; next < f->len && nopen; ++next)
		{
			nopen += f->buf[next].codepoint == '<';
			nopen -= f->buf[next].codepoint == '>';
		}
	}
	
	while (next < f->len && e_isspace(f->buf[next]))
	{
		++next;
	}
	
	if (next < f->len && f->buf[next].codepoint == '(')
	{
		r->fg = o_opts.emphfg;
		r->bg = o_opts.emphbg;
		return;
	}
	
	r->fg = o_opts.normfg;
	r->bg = o_opts.normbg;
}
