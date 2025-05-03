// SPDX-License-Identifier: GPL-3.0-or-later

#define O_K(k) \
	{ \
		.enc[0] = k, \
		.codepoint = k \
	}

#define O_KEND \
	{ \
		.codepoint = E_INVALIDCODEPOINT \
	}

#define O_KCTL(k) O_K(k - 'a' + 1)
#define O_KFN(n) O_K(27), O_K(79), O_K(79 + n)
#define O_KMETA(k) O_K(27), O_K(k)

// keybind options.
e_char o_bmvleft[] = {O_K('h'), O_KEND};
e_char o_bmvright[] = {O_K('l'), O_KEND};
e_char o_bmvup[] = {O_K('k'), O_KEND};
e_char o_bmvdown[] = {O_K('j'), O_KEND};
e_char o_bmvstart[] = {O_K('a'), O_KEND};
e_char o_bmvend[] = {O_K('e'), O_KEND};
e_char o_bquit[] = {O_KCTL('x'), O_KEND};

o_options o_opts;

static FILE *o_openconf(char const *file);
static i32 o_getraw(char const *name, FILE *fp, char const *key, OUT char val[]);
static i32 o_getu32(char const *name, FILE *fp, char const *key, OUT u32 *val);
static i32 o_getu8(char const *name, FILE *fp, char const *key, OUT u8 *val);

i32
o_parse(void)
{
	FILE *fp = o_openconf(O_MAINCONF);
	if (!fp)
	{
		return 1;
	}
	
	if (o_getu32(O_MAINCONF, fp, "masternum", &o_opts.masternum)
		|| o_getu32(O_MAINCONF, fp, "masterdenom", &o_opts.masterdenom)
		|| o_getu32(O_MAINCONF, fp, "lgutter", &o_opts.lgutter)
		|| o_getu32(O_MAINCONF, fp, "rgutter", &o_opts.rgutter)
		|| o_getu32(O_MAINCONF, fp, "margin", &o_opts.margin)
		|| o_getu32(O_MAINCONF, fp, "tab", &o_opts.tab)
		|| o_getu8(O_MAINCONF, fp, "globalfg", &o_opts.globalfg)
		|| o_getu8(O_MAINCONF, fp, "globalbg", &o_opts.globalbg)
		|| o_getu8(O_MAINCONF, fp, "wndfg", &o_opts.wndfg)
		|| o_getu8(O_MAINCONF, fp, "wndbg", &o_opts.wndbg)
		|| o_getu8(O_MAINCONF, fp, "curwndfg", &o_opts.curwndfg)
		|| o_getu8(O_MAINCONF, fp, "curwndbg", &o_opts.curwndbg)
		|| o_getu8(O_MAINCONF, fp, "normfg", &o_opts.normfg)
		|| o_getu8(O_MAINCONF, fp, "normbg", &o_opts.normbg)
		|| o_getu8(O_MAINCONF, fp, "linumfg", &o_opts.linumfg)
		|| o_getu8(O_MAINCONF, fp, "linumbg", &o_opts.linumbg)
		|| o_getu8(O_MAINCONF, fp, "marginfg", &o_opts.marginfg)
		|| o_getu8(O_MAINCONF, fp, "marginbg", &o_opts.marginbg)
		|| o_getu8(O_MAINCONF, fp, "csrfg", &o_opts.csrfg)
		|| o_getu8(O_MAINCONF, fp, "csrbg", &o_opts.csrbg)
		|| o_getu8(O_MAINCONF, fp, "hlfg", &o_opts.hlfg)
		|| o_getu8(O_MAINCONF, fp, "hlbg", &o_opts.hlbg))
	{
		fclose(fp);
		return 1;
	}
	
	fclose(fp);
	return 0;
}

static FILE *
o_openconf(char const *file)
{
	char const *home = getenv("HOME");
	if (!home)
	{
		struct passwd *pw = getpwuid(getuid());
		if (!pw)
		{
			showerr("options: failed on getpwuid() getting home directory!");
			return NULL;
		}
		
		home = pw->pw_dir;
	}
	
	char path[PATH_MAX + 1] = {0};
	snprintf(path, PATH_MAX, "%s/%s/%s", home, O_CONFDIR, file);
	
	FILE *fp = fopen(path, "rb");
	if (!fp)
	{
		showerr("options: failed to open conf file - %s!", path);
		return NULL;
	}
	
	return fp;
}

static i32
o_getraw(char const *name, FILE *fp, char const *key, OUT char val[])
{
	fseek(fp, 0, SEEK_SET);
	
	for (usize line = 0; !feof(fp) && !ferror(fp); ++line)
	{
		i32 ch;
		while (ch = fgetc(fp), ch != EOF && isspace(ch))
		{
		}
		
		if (ch == '#')
		{
			while (ch = fgetc(fp), ch != EOF && ch != '\n')
			{
			}
		}
		
		if (ch == '\n' || feof(fp))
		{
			continue;
		}
		
		fseek(fp, -1, SEEK_CUR);
		char buf[O_CONFKEYLEN] = {0};
		if (fscanf(fp, O_CONFSCAN, buf, val) != 2)
		{
			showerr("options: error on line %zu of %s!", line, name);
			return 1;
		}
		
		if (!strcmp(val, "NONE"))
		{
			val[0] = 0;
		}
		
		if (!strcmp(buf, key))
		{
			return 0;
		}
	}
	
	showerr("options: didn't find %s in %s!", key, name);
	return 1;
}

static i32
o_getu32(char const *name, FILE *fp, char const *key, OUT u32 *val)
{
	char buf[O_CONFVALLEN] = {0};
	if (o_getraw(name, fp, key, buf))
	{
		return 1;
	}
	
	errno = 0;
	unsigned long long ull = strtoull(buf, NULL, 0);
	if (errno || ull > UINT32_MAX)
	{
		showerr("options: invalid u32 value for %s in %s!", key, name);
		return 1;
	}
	
	*val = ull;
	return 0;
}

static i32
o_getu8(char const *name, FILE *fp, char const *key, OUT u8 *val)
{
	char buf[O_CONFVALLEN] = {0};
	if (o_getraw(name, fp, key, buf))
	{
		return 1;
	}
	
	errno = 0;
	unsigned long long ull = strtoull(buf, NULL, 0);
	if (errno || ull > UINT8_MAX)
	{
		showerr("options: invalid u8 value for %s in %s!", key, name);
		return 1;
	}
	
	*val = ull;
	return 0;
}
