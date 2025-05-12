// SPDX-License-Identifier: GPL-3.0-or-later

e_char_t
e_read(void)
{
	return e_fread(stdin);
}

e_char_t
e_fread(FILE *fp)
{
	u8 enc[4] = {0};
	
	enc[0] = fgetc(fp);
	if (enc[0] < 0x80)
	{
		return (e_char_t)
		{
			.codepoint = enc[0],
			.enc = {enc[0], 0, 0, 0}
		};
	}
	
	i32 nbytes = 1;
	for (i32 i = 6; i >= 0; --i)
	{
		if (!(enc[0] & 1 << i))
		{
			break;
		}
		++nbytes;
	}
	
	u32 codepoint = 0;
	switch (nbytes)
	{
	case 2:
		enc[1] = fgetc(fp);
		
		codepoint |= (enc[0] & 0x1f) << 6;
		codepoint |= enc[1] & 0x3f;
		
		break;
	case 3:
		enc[1] = fgetc(fp);
		enc[2] = fgetc(fp);
		
		codepoint |= (enc[0] & 0xf) << 12;
		codepoint |= (enc[1] & 0x3f) << 6;
		codepoint |= enc[2] & 0x3f;
		
		break;
	case 4:
		enc[1] = fgetc(fp);
		enc[2] = fgetc(fp);
		enc[3] = fgetc(fp);
		
		codepoint |= (enc[0] & 0x7) << 18;
		codepoint |= (enc[1] & 0x3f) << 12;
		codepoint |= (enc[2] & 0x3f) << 6;
		codepoint |= enc[3] & 0x3f;
		
		break;
	default:
		return e_fromcodepoint(E_REPLACEMENT);
	}
	
	return (e_char_t)
	{
		.codepoint = codepoint,
		.enc = {enc[0], enc[1], enc[2], enc[3]}
	};
}

e_char_t
e_fromcodepoint(u32 codepoint)
{
	e_char_t ch =
	{
		.codepoint = codepoint
	};
	
	if (codepoint < 0x80)
	{
		ch.enc[0] = codepoint;
	}
	else if (codepoint < 0x800)
	{
		ch.enc[0] = 0xc0 | (codepoint & 0x7c0);
		ch.enc[1] = 0x80 | (codepoint & 0x3f);
	}
	else if (codepoint < 0x10000)
	{
		ch.enc[0] = 0xe0 | (codepoint & 0xf000);
		ch.enc[1] = 0x80 | (codepoint & 0xfc0);
		ch.enc[2] = 0x80 | (codepoint & 0x3f);
	}
	else if (codepoint < 0x11000)
	{
		ch.enc[0] = 0xf0 | (codepoint & 0x1c0000);
		ch.enc[1] = 0x80 | (codepoint & 0x3f000);
		ch.enc[2] = 0x80 | (codepoint & 0xfc0);
		ch.enc[3] = 0x80 | (codepoint & 0x3f);
	}
	else
	{
		ch = e_fromcodepoint(E_REPLACEMENT);
	}
	
	return ch;
}

e_char_t *
e_fromstr(OUT usize *len, char const *s)
{
	*len = strlen(s);
	e_char_t *es = calloc(*len, sizeof(e_char_t));
	
	for (usize i = 0; i < *len;)
	{
		usize nbytes = e_frommem(&es[i], (u8 const *)&s[i]);
		i += nbytes ? nbytes : 1;
	}
	
	return es;
}

char *
e_tostr(e_char_t const *data, usize len)
{
	// first allocate too much memory, then reallocate to the appropriate amount.
	char *str = calloc(len + 1, sizeof(e_char_t));
	usize slen = 0;
	
	for (usize i = 0; i < len; ++i)
	{
		for (usize j = 0; j < 4; ++j)
		{
			if (!data[i].enc[j])
			{
				break;
			}
			
			str[slen++] = data[i].enc[j];
		}
	}
	
	return realloc(str, slen + 1);
}

usize
e_frommem(OUT e_char_t *ch, u8 const *p)
{
	if (p[0] < 0x80)
	{
		*ch = (e_char_t)
		{
			.codepoint = p[0],
			.enc = {p[0], 0, 0, 0}
		};
		return 1;
	}
	
	i32 nbytes = 1;
	for (i32 i = 6; i >= 0; --i)
	{
		if (!(p[0] & 1 << i))
		{
			break;
		}
		++nbytes;
	}
	
	u32 codepoint = 0;
	switch (nbytes)
	{
	case 2:
		codepoint |= (p[0] & 0x1f) << 6;
		codepoint |= p[1] & 0x3f;
		break;
	case 3:
		codepoint |= (p[0] & 0xf) << 12;
		codepoint |= (p[1] & 0x3f) << 6;
		codepoint |= p[2] & 0x3f;
		break;
	case 4:
		codepoint |= (p[0] & 0x7) << 18;
		codepoint |= (p[1] & 0x3f) << 12;
		codepoint |= (p[2] & 0x3f) << 6;
		codepoint |= p[3] & 0x3f;
		break;
	default:
		*ch = e_fromcodepoint(E_REPLACEMENT);
		return 0;
	}
	
	*ch = (e_char_t)
	{
		.codepoint = codepoint,
		.enc = {p[0], p[1], p[2], p[3]}
	};
	return nbytes;
}

i32
e_putch(e_char_t ch)
{
	return e_fputch(ch, stdout);
}

i32
e_fputch(e_char_t ch, FILE *fp)
{
	for (usize i = 0; i < 4; ++i)
	{
		if (!ch.enc[i])
		{
			return 0;
		}
		
		if (fprintf(fp, "%c", ch.enc[i]) < 0)
		{
			return 1;
		}
	}
	
	return 0;
}

bool
e_isprint(e_char_t ch)
{
	u32 cp = ch.codepoint;
	
	if (cp <= 0x1f
		|| (cp >= 0x7f && cp <= 0x9f)
		|| cp == 0x61c
		|| (cp >= 0x200e && cp <= 0x200f)
		|| (cp >= 0x2028 && cp <= 0x2029)
		|| (cp >= 0x202a && cp <= 0x202e)
		|| (cp >= 0x2066 && cp <= 0x2069)
		|| (cp >= 0xfff9 && cp <= 0xfffb)
		|| (cp >= 0xe0000 && cp <= 0xe007f))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool
e_isspace(e_char_t ch)
{
	u32 cp = ch.codepoint;
	
	if ((cp >= 0x9 && cp <= 0xd)
		|| cp == 0x20
		|| cp == 0x85
		|| cp == 0xa0
		|| cp == 0x1680
		|| (cp >= 0x2000 && cp <= 0x200a)
		|| (cp >= 0x2028 && cp <= 0x2029)
		|| cp == 0x202f
		|| cp == 0x205f
		|| cp == 0x3000)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
e_isalpha(e_char_t ch)
{
	u32 cp = ch.codepoint;
	
	// some writing systems are not included.
	if ((cp >= 0x41 && cp <= 0x5a)
		|| (cp >= 0x61 && cp <= 0x7a)
		|| (cp >= 0xc0 && cp <= 0xd6)
		|| (cp >= 0xd8 && cp <= 0xf6)
		|| (cp >= 0xf8 && cp <= 0x2af)
		|| (cp >= 0x370 && cp <= 0x4ff)
		|| (cp >= 0x530 && cp <= 0x74f)
		|| (cp >= 0x780 && cp <= 0x7bf)
		|| (cp >= 0x900 && cp <= 0xdff)
		|| (cp >= 0x10a0 && cp <= 0x137f)
		|| (cp >= 0x1400 && cp <= 0x167f)
		|| (cp >= 0x1800 && cp <= 0x18af)
		|| (cp >= 0x1e00 && cp <= 0x1fff)
		|| (cp >= 0x3040 && cp <= 0x33ff)
		|| (cp >= 0xfb00 && cp <= 0xfb4f)
		|| (cp >= 0xfe30 && cp <= 0xfe4f)
		|| (cp >= 0x10450 && cp <= 0x1047f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
e_isdigit(e_char_t ch)
{
	u32 cp = ch.codepoint;
	
	if ((cp >= 0x30 && cp <= 0x39)
		|| (cp >= 0x2160 && cp <= 0x2188)
		|| cp == 0x3007
		|| (cp >= 0x3021 && cp <= 0x3029)
		|| cp == 0x5341
		|| (cp >= 0x5344 && cp <= 0x5345)
		|| (cp >= 0x10140 && cp <= 0x1018f)
		|| (cp >= 0x1d360 && cp <= 0x1d37f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
e_isalnum(e_char_t ch)
{
	return e_isalpha(ch) || e_isdigit(ch);
}
