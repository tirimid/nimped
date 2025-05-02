// SPDX-License-Identifier: GPL-3.0-or-later

e_char
e_read(void)
{
	return e_fread(stdin);
}

e_char
e_fread(FILE *fp)
{
	u8 enc[4] = {0};
	
	enc[0] = fgetc(fp);
	if (enc[0] < 0x80)
	{
		return (e_char)
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
		codepoint = E_INVALIDCODEPOINT;
		break;
	}
	
	return (e_char)
	{
		.codepoint = codepoint,
		.enc = {enc[0], enc[1], enc[2], enc[3]}
	};
}

e_char
e_fromcodepoint(u32 codepoint)
{
	e_char ch =
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
		ch.codepoint = E_INVALIDCODEPOINT;
	}
	
	return ch;
}

e_char *
e_fromstr(OUT usize *len, char const *s)
{
	*len = strlen(s);
	e_char *es = calloc(*len, sizeof(e_char));
	
	for (usize i = 0; i < *len;)
	{
		usize nbytes = e_frommem(&es[i], (u8 const *)&s[i]);
		i += nbytes ? nbytes : 1;
	}
	
	return es;
}

usize
e_frommem(OUT e_char *ch, u8 const *p)
{
	if (p[0] < 0x80)
	{
		*ch = (e_char)
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
		nbytes = 0;
		codepoint = E_INVALIDCODEPOINT;
		break;
	}
	
	*ch = (e_char)
	{
		.codepoint = codepoint,
		.enc = {p[0], p[1], p[2], p[3]}
	};
	return nbytes;
}

i32
e_putch(e_char ch)
{
	return e_fputch(ch, stdout);
}

i32
e_fputch(e_char ch, FILE *fp)
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
