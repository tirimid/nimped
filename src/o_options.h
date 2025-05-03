// SPDX-License-Identifier: GPL-3.0-or-later

// internal options.
#define O_MAXLOGLEN 512
#define O_CONFKEYLEN 128
#define O_CONFVALLEN 128
#define O_CONFSCAN "%127s = %127[^\r\n]"

// functional options.
#define O_MAXFILES 8
#define O_CONFDIR ".config/nimped"
#define O_LANGDIR "lang"
#define O_MAINCONF "nimped.conf"

// input options.
#define O_MAXBINDLEN 16
#define O_MAXBINDS 128

// frame options.
#define O_SCRATCHNAME "-scratch-"
#define O_GREETERTEXT \
	"nimped - (n)ew and (imp)roved text (ed)itor\n" \
	"\n" \
	"           _                          _\n" \
	"     _ __ (_)_ __ ___  _ __   ___  __| |\n" \
	"    | '_ \\| | '_ ` _ \\| '_ \\ / _ \\/ _` |\n" \
	"    | | | | | | | | | | |_) |  __/ (_| |\n" \
	"    |_| |_|_|_| |_| |_| .__/ \\___|\\__,_|\n" \
	"                      |_|\n" \
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
	"* Source code: https://git.tirimid.net/nimped\n" \
	"\n" \
	"(The greeter logo seen above was generated with the use of Figlet)\n"

// visual options.
#define O_MARGINCHAR '|'

// keybind options.
extern e_char o_bmvleft[];
extern e_char o_bmvright[];
extern e_char o_bmvup[];
extern e_char o_bmvdown[];
extern e_char o_bmvstart[];
extern e_char o_bmvend[];
extern e_char o_bquit[];

// dynamic options.
typedef struct o_options
{
	u32 masternum, masterdenom;
	u32 lgutter, rgutter;
	u32 margin;
	u32 tab;
	u8 globalfg, globalbg;
	u8 wndfg, wndbg;
	u8 curwndfg, curwndbg;
	u8 normfg, normbg;
	u8 linumfg, linumbg;
	u8 marginfg, marginbg;
	u8 csrfg, csrbg;
	u8 hlfg, hlbg;
} o_options;

extern o_options o_opts;

i32 o_parse(void);
