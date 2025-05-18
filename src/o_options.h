// SPDX-License-Identifier: GPL-3.0-or-later

// internal options.
#define O_MAXLOGLEN 512
#define O_CONFKEYLEN 128
#define O_CONFVALLEN 128
#define O_CONFSCAN "%127s = %127[^\r\n]"

// functional options.
#define O_MAXFILES 8
#define O_CONFDIR ".config/nimped"
#define O_LAYOUTCONF "layout.conf"
#define O_COLORCONF "color.conf"
#define O_LANGCONF "lang.conf"
#define O_MAXBARLEN 512
#define O_MAXKEYWORDS 128
#define O_MAXPROMPTLEN 512
#define O_MAXBINDLEN 16
#define O_MAXBINDS 128

// frame options.
#define O_GREETERTEXT \
	"nimped - (n)ew and (imp)roved text (ed)itor\n" \
	"\n" \
	"\t       _                          _\n" \
	"\t _ __ (_)_ __ ___  _ __   ___  __| |\n" \
	"\t| '_ \\| | '_ ` _ \\| '_ \\ / _ \\/ _` |\n" \
	"\t| | | | | | | | | | |_) |  __/ (_| |\n" \
	"\t|_| |_|_|_| |_| |_| .__/ \\___|\\__,_|\n" \
	"\t                  |_|\n" \
	"\n" \
	"Welcome to the new and improved text editor by Tirimid - nimped.\n" \
	"\n" \
	"This text editor was created in order to improve upon my previous text\n" \
	"editor, medioed, with a somewhat different design philosophy. Just like\n" \
	"medioed, this program will always be Free Software, licensed under the\n" \
	"GNU GPLv3, for the benefit of users and those wishing to study it. It\n" \
	"was created for my personal use, but you are free to use it too.\n" \
	"\n" \
	"Additional resources:\n" \
	"\n" \
	"\tSource code: https://git.tirimid.net/nimped\n" \
	"\tWriteup and docs: https://tirimid.net/tirimid/nimped.html\n" \
	"\n" \
	"(The greeter logo seen above was generated with the use of Figlet)\n"

// visual options.
#define O_MARGINCHAR '|'
#define O_SCRATCHNAME "-scratch-"
#define O_BASENAME "-base-"
#define O_WRITENAME "-write-"

typedef enum o_langmode
{
	O_CMODE = 0,
	O_SHMODE,
	O_JSMODE,
	
	O_LANGMODE_END
} o_langmode_t;

// keybind options.
extern e_char_t o_bfmvleft[];
extern e_char_t o_bfmvright[];
extern e_char_t o_bfmvup[];
extern e_char_t o_bfmvdown[];
extern e_char_t o_bfmvstart[];
extern e_char_t o_bfmvend[];
extern e_char_t o_bfmvwordleft[];
extern e_char_t o_bfmvwordright[];
extern e_char_t o_bpmvleft[];
extern e_char_t o_bpmvright[];
extern e_char_t o_bpmvstart[];
extern e_char_t o_bpmvend[];
extern e_char_t o_bpmvwordleft[];
extern e_char_t o_bpmvwordright[];
extern e_char_t o_bquit[];
extern e_char_t o_bnext[];
extern e_char_t o_bprev[];
extern e_char_t o_bwritemode[];
extern e_char_t o_bdelfront[];
extern e_char_t o_bdelback[];
extern e_char_t o_bdelword[];
extern e_char_t o_bnewline[];
extern e_char_t o_bundo[];
extern e_char_t o_bnewframe[];
extern e_char_t o_bkillframe[];
extern e_char_t o_bsave[];
extern e_char_t o_bfocus[];
extern e_char_t o_bopenfile[];
extern e_char_t o_bcomplete[];
extern e_char_t o_bsearch[];
extern e_char_t o_brevsearch[];
extern e_char_t o_bleftparen[];
extern e_char_t o_bleftbracket[];
extern e_char_t o_bleftbrace[];
extern e_char_t o_bdoublequote[];
extern e_char_t o_bpaste[];
extern e_char_t o_bcopyline[];
extern e_char_t o_bcutline[];
extern e_char_t o_bncopyline[];
extern e_char_t o_bncutline[];
extern e_char_t o_bzoom[];
extern e_char_t o_bgoto[];
extern e_char_t o_brecmacro[];
extern e_char_t o_bexecmacro[];

typedef struct o_opts
{
	// layout options.
	u32 masternum, masterdenom;
	u32 lgutter, rgutter;
	u32 margin;
	u32 tab;
	
	// editor color options.
	u8 globalfg, globalbg;
	u8 wndfg, wndbg;
	u8 curwndfg, curwndbg;
	u8 normfg, normbg;
	u8 linumfg, linumbg;
	u8 marginfg, marginbg;
	u8 csrfg, csrbg, csrhlbg;
	u8 linumhlfg, linumhlbg;
	
	// highlight color options.
	u8 commentfg, commentbg;
	u8 macrofg, macrobg;
	u8 specialfg, specialbg;
	u8 keywordfg, keywordbg;
	u8 primitivefg, primitivebg;
	u8 typefg, typebg;
	u8 emphfg, emphbg;
	u8 stringfg, stringbg;
	u8 numberfg, numberbg;
	
	// language mode options.
	struct
	{
		// control keywords.
		usize nkeywords;
		e_char_t *keywords[O_MAXKEYWORDS];
		u16 keywordlen[O_MAXKEYWORDS];
		
		// primitive type keywords (although you may use it for something else).
		usize nprimitives;
		e_char_t *primitives[O_MAXKEYWORDS];
		u16 primitivelen[O_MAXKEYWORDS];
	} lang[O_LANGMODE_END];
} o_opts_t;

extern o_opts_t o_opts;

i32 o_parse(void);
