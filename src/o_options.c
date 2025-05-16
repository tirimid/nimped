// SPDX-License-Identifier: GPL-3.0-or-later

#define O_K(k) \
	{ \
		.enc = {k, 0, 0, 0}, \
		.codepoint = k \
	}

#define O_KEND O_K(0)

#define O_KCTL(k) O_K(k - 'a' + 1)
#define O_KFN(n) O_K(27), O_K(79), O_K(79 + n)
#define O_KMETA(k) O_K(27), O_K(k)

typedef struct o_namedcolor
{
	char const *name;
	u8 col;
} o_namedcolor_t;

// keybind options.
e_char_t o_bfmvleft[] = {O_K('h'), O_KEND};
e_char_t o_bfmvright[] = {O_K('l'), O_KEND};
e_char_t o_bfmvup[] = {O_K('k'), O_KEND};
e_char_t o_bfmvdown[] = {O_K('j'), O_KEND};
e_char_t o_bfmvstart[] = {O_K('a'), O_KEND};
e_char_t o_bfmvend[] = {O_K('e'), O_KEND};
e_char_t o_bfmvwordleft[] = {O_K('b'), O_KEND};
e_char_t o_bfmvwordright[] = {O_K('f'), O_KEND};
e_char_t o_bpmvleft[] = {O_KCTL('b'), O_KEND};
e_char_t o_bpmvright[] = {O_KCTL('f'), O_KEND};
e_char_t o_bpmvstart[] = {O_KCTL('a'), O_KEND};
e_char_t o_bpmvend[] = {O_KCTL('e'), O_KEND};
e_char_t o_bpmvwordleft[] = {O_KMETA('b'), O_KEND};
e_char_t o_bpmvwordright[] = {O_KMETA('f'), O_KEND};
e_char_t o_bquit[] = {O_KCTL('x'), O_KEND};
e_char_t o_bnext[] = {O_K('n'), O_KEND};
e_char_t o_bprev[] = {O_K('p'), O_KEND};
e_char_t o_bwritemode[] = {O_K('i'), O_KEND};
e_char_t o_bdelfront[] = {O_KCTL('d'), O_KEND};
e_char_t o_bdelback[] = {O_K(127), O_KEND};
e_char_t o_bdelword[] = {O_KMETA(127), O_KEND};
e_char_t o_bnewline[] = {O_K(13), O_KEND};
e_char_t o_bundo[] = {O_K('u'), O_KEND};
e_char_t o_bnewframe[] = {O_KCTL('n'), O_KEND};
e_char_t o_bkillframe[] = {O_KCTL('k'), O_KEND};
e_char_t o_bsave[] = {O_KCTL('s'), O_KEND};
e_char_t o_bfocus[] = {O_K('m'), O_KEND};
e_char_t o_bopenfile[] = {O_KCTL('f'), O_KEND};
e_char_t o_bcomplete[] = {O_K(9), O_KEND};
e_char_t o_bsearch[] = {O_K('/'), O_KEND};
e_char_t o_brevsearch[] = {O_K('?'), O_KEND};
e_char_t o_bleftparen[] = {O_K('('), O_KEND};
e_char_t o_bleftbracket[] = {O_K('['), O_KEND};
e_char_t o_bleftbrace[] = {O_K('{'), O_KEND};
e_char_t o_bdoublequote[] = {O_K('"'), O_KEND};
e_char_t o_bpaste[] = {O_K('v'), O_KEND};
e_char_t o_bcopyline[] = {O_K('c'), O_KEND};
e_char_t o_bcutline[] = {O_K('d'), O_KEND};
e_char_t o_bncopyline[] = {O_K('q'), O_K('c'), O_KEND};
e_char_t o_bncutline[] = {O_K('q'), O_K('d'), O_KEND};
e_char_t o_bzoom[] = {O_K('z'), O_KEND};
e_char_t o_bgoto[] = {O_K('g'), O_KEND};
e_char_t o_brecmacro[] = {O_KFN(3), O_KEND};
e_char_t o_bexecmacro[] = {O_KFN(4), O_KEND};

o_opts_t o_opts;

static FILE *o_openconf(char const *file);
static i32 o_getraw(char const *name, FILE *fp, char const *key, OUT char val[]);
static i32 o_getu32(char const *name, FILE *fp, char const *key, OUT u32 *val);
static i32 o_getcolor(char const *name, FILE *fp, char const *key, OUT u8 *val);
static bool o_nthraw(char const *name, FILE *fp, char const *key, OUT char val[], i32 n);
static void o_readlangconf(FILE *fp, char const *kwkey, char const *primkey, o_langmode_t mode);

// these colors will be usable as named palette colors in config.
// add the palettes of your favorite themes here.
static o_namedcolor_t o_namedcolors[] =
{
	// terminal.
	{"@t.darkbg", 0},
	{"@t.darkred", 1},
	{"@t.darkgreen", 2},
	{"@t.darkyellow", 3},
	{"@t.darkblue", 4},
	{"@t.darkpurple", 5},
	{"@t.darkaqua", 6},
	{"@t.darkfg", 7},
	{"@t.lightbg", 8},
	{"@t.lightred", 9},
	{"@t.lightgreen", 10},
	{"@t.lightyellow", 11},
	{"@t.lightblue", 12},
	{"@t.lightpurple", 13},
	{"@t.lightaqua", 14},
	{"@t.lightfg", 15},
	
	// gruvbox dark.
	{"@gbd.bg", 235},
	{"@gbd.darkred", 124},
	{"@gbd.darkgreen", 106},
	{"@gbd.darkyellow", 172},
	{"@gbd.darkblue", 66},
	{"@gbd.darkpurple", 132},
	{"@gbd.darkaqua", 72},
	{"@gbd.lightgray", 246},
	{"@gbd.darkgray", 245},
	{"@gbd.lightred", 167},
	{"@gbd.lightgreen", 142},
	{"@gbd.lightyellow", 214},
	{"@gbd.lightblue", 109},
	{"@gbd.lightpurple", 175},
	{"@gbd.lightaqua", 108},
	{"@gbd.fg", 223},
	{"@gbd.bg0_h", 234},
	{"@gbd.bg1", 237},
	{"@gbd.bg2", 239},
	{"@gbd.bg3", 241},
	{"@gbd.bg4", 243},
	{"@gbd.darkorange", 166},
	{"@gbd.bg0_s", 236},
	{"@gbd.fg4", 246},
	{"@gbd.fg3", 248},
	{"@gbd.fg2", 250},
	{"@gbd.fg1", 223},
	{"@gbd.fg0", 229},
	{"@gbd.lightorange", 208},
	
	// github dark.
	{"@ghd.base0", 233},
	{"@ghd.base1", 235},
	{"@ghd.base2", 237},
	{"@ghd.base3", 243},
	{"@ghd.base4", 249},
	{"@ghd.base5", 252},
	{"@ghd.lightred", 210},
	{"@ghd.darkred", 203},
	{"@ghd.darkyellow", 178},
	{"@ghd.lightyellow", 221},
	{"@ghd.lightblue", 153},
	{"@ghd.darkblue", 75},
	{"@ghd.lightpurple", 183},
	{"@ghd.darkpurple", 135},
	{"@ghd.lightgreen", 114},
	{"@ghd.darkgreen", 76},
	
	// vs dark.
	{"@vsd.bg", 233},
	{"@vsd.fg0", 188},
	{"@vsd.fg1", 108},
	{"@vsd.beige", 180},
	{"@vsd.aqua0", 50},
	{"@vsd.aqua1", 108},
	{"@vsd.purple", 169},
	{"@vsd.green", 64},
	{"@vsd.blue0", 152},
	{"@vsd.blue1", 30},
	{"@vsd.blue2", 39},
	{"@vsd.yellow", 230}
};

i32
o_parse(void)
{
	FILE *fp = o_openconf(O_LAYOUTCONF);
	if (!fp)
	{
		return 1;
	}
	
	// layout conf options.
	if (o_getu32(O_LAYOUTCONF, fp, "masternum", &o_opts.masternum)
		|| o_getu32(O_LAYOUTCONF, fp, "masterdenom", &o_opts.masterdenom)
		|| o_getu32(O_LAYOUTCONF, fp, "lgutter", &o_opts.lgutter)
		|| o_getu32(O_LAYOUTCONF, fp, "rgutter", &o_opts.rgutter)
		|| o_getu32(O_LAYOUTCONF, fp, "margin", &o_opts.margin)
		|| o_getu32(O_LAYOUTCONF, fp, "tab", &o_opts.tab))
	{
		fclose(fp);
		return 1;
	}
	
	fclose(fp);
	fp = o_openconf(O_COLORCONF);
	if (!fp)
	{
		return 1;
	}
	
	// color conf options.
	if (o_getcolor(O_COLORCONF, fp, "globalfg", &o_opts.globalfg)
		|| o_getcolor(O_COLORCONF, fp, "globalbg", &o_opts.globalbg)
		|| o_getcolor(O_COLORCONF, fp, "wndfg", &o_opts.wndfg)
		|| o_getcolor(O_COLORCONF, fp, "wndbg", &o_opts.wndbg)
		|| o_getcolor(O_COLORCONF, fp, "curwndfg", &o_opts.curwndfg)
		|| o_getcolor(O_COLORCONF, fp, "curwndbg", &o_opts.curwndbg)
		|| o_getcolor(O_COLORCONF, fp, "normfg", &o_opts.normfg)
		|| o_getcolor(O_COLORCONF, fp, "normbg", &o_opts.normbg)
		|| o_getcolor(O_COLORCONF, fp, "linumfg", &o_opts.linumfg)
		|| o_getcolor(O_COLORCONF, fp, "linumbg", &o_opts.linumbg)
		|| o_getcolor(O_COLORCONF, fp, "marginfg", &o_opts.marginfg)
		|| o_getcolor(O_COLORCONF, fp, "marginbg", &o_opts.marginbg)
		|| o_getcolor(O_COLORCONF, fp, "csrfg", &o_opts.csrfg)
		|| o_getcolor(O_COLORCONF, fp, "csrbg", &o_opts.csrbg)
		|| o_getcolor(O_COLORCONF, fp, "csrhlbg", &o_opts.csrhlbg)
		|| o_getcolor(O_COLORCONF, fp, "linumhlfg", &o_opts.linumhlfg)
		|| o_getcolor(O_COLORCONF, fp, "linumhlbg", &o_opts.linumhlbg)
		|| o_getcolor(O_COLORCONF, fp, "commentfg", &o_opts.commentfg)
		|| o_getcolor(O_COLORCONF, fp, "commentbg", &o_opts.commentbg)
		|| o_getcolor(O_COLORCONF, fp, "macrofg", &o_opts.macrofg)
		|| o_getcolor(O_COLORCONF, fp, "macrobg", &o_opts.macrobg)
		|| o_getcolor(O_COLORCONF, fp, "specialfg", &o_opts.specialfg)
		|| o_getcolor(O_COLORCONF, fp, "specialbg", &o_opts.specialbg)
		|| o_getcolor(O_COLORCONF, fp, "keywordfg", &o_opts.keywordfg)
		|| o_getcolor(O_COLORCONF, fp, "keywordbg", &o_opts.keywordbg)
		|| o_getcolor(O_COLORCONF, fp, "primitivefg", &o_opts.primitivefg)
		|| o_getcolor(O_COLORCONF, fp, "primitivebg", &o_opts.primitivebg)
		|| o_getcolor(O_COLORCONF, fp, "typefg", &o_opts.typefg)
		|| o_getcolor(O_COLORCONF, fp, "typebg", &o_opts.typebg)
		|| o_getcolor(O_COLORCONF, fp, "emphfg", &o_opts.emphfg)
		|| o_getcolor(O_COLORCONF, fp, "emphbg", &o_opts.emphbg)
		|| o_getcolor(O_COLORCONF, fp, "stringfg", &o_opts.stringfg)
		|| o_getcolor(O_COLORCONF, fp, "stringbg", &o_opts.stringbg)
		|| o_getcolor(O_COLORCONF, fp, "numberfg", &o_opts.numberfg)
		|| o_getcolor(O_COLORCONF, fp, "numberbg", &o_opts.numberbg))
	{
		fclose(fp);
		return 1;
	}
	
	fclose(fp);
	fp = o_openconf(O_LANGCONF);
	if (!fp)
	{
		return 1;
	}
	
	// lang conf options.
	o_readlangconf(fp, "ckeyword", "cprimitive", O_CMODE);
	o_readlangconf(fp, "shkeyword", "shprimitive", O_SHMODE);
	o_readlangconf(fp, "jskeyword", "jsprimitive", O_JSMODE);
	
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
o_getcolor(char const *name, FILE *fp, char const *key, OUT u8 *val)
{
	char buf[O_CONFVALLEN] = {0};
	if (o_getraw(name, fp, key, buf))
	{
		return 1;
	}
	
	// named color palettes.
	if (buf[0] == '@')
	{
		for (usize i = 0; i < sizeof(o_namedcolors) / sizeof(o_namedcolors[0]); ++i)
		{
			if (!strcmp(buf, o_namedcolors[i].name))
			{
				*val = o_namedcolors[i].col;
				return 0;
			}
		}
		
		showerr("options: unknown color value for %s in %s!", key, name);
		return 1;
	}
	
	errno = 0;
	unsigned long long ull = strtoull(buf, NULL, 0);
	if (errno || ull > UINT8_MAX)
	{
		showerr("options: invalid color value for %s in %s!", key, name);
		return 1;
	}
	
	*val = ull;
	return 0;
}

static bool
o_nthraw(char const *name, FILE *fp, char const *key, OUT char val[], i32 n)
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
			return false;
		}
		
		if (!strcmp(val, "NONE"))
		{
			val[0] = 0;
		}
		
		if (!strcmp(buf, key))
		{
			if (n > 0)
			{
				--n;
				continue;
			}
			return true;
		}
	}
	
	return false;
}

static void
o_readlangconf(
	FILE *fp,
	char const *kwkey,
	char const *primkey,
	o_langmode_t mode
)
{
	char val[O_CONFVALLEN] = {0};
	
	for (i32 i = 0; o_nthraw(O_LANGCONF, fp, kwkey, val, i); ++i)
	{
		usize *nkw = &o_opts.lang[mode].nkeywords;
		if (*nkw >= O_MAXKEYWORDS)
		{
			break;
		}
		
		usize kwlen;
		e_char_t *kw = e_fromstr(&kwlen, val);
		
		o_opts.lang[mode].keywords[*nkw] = kw;
		o_opts.lang[mode].keywordlen[*nkw] = kwlen;
		++*nkw;
	}
	
	for (i32 i = 0; o_nthraw(O_LANGCONF, fp, primkey, val, i); ++i)
	{
		usize *nprim = &o_opts.lang[mode].nprimitives;
		if (*nprim >= O_MAXKEYWORDS)
		{
			break;
		}
		
		usize primlen;
		e_char_t *prim = e_fromstr(&primlen, val);
		
		o_opts.lang[mode].primitives[*nprim] = prim;
		o_opts.lang[mode].primitivelen[*nprim] = primlen;
		++*nprim;
	}
}
