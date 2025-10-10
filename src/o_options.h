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
#define O_EDITINGCONF "editing.conf"
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
	"Resources:\n" \
	"\n" \
	"\tSource code: https://git.tirimid.net/nimped\n" \
	"\tWriteup and docs: https://tirimid.net/tirimid/nimped.html\n" \
	"\tPress C-h to bring up help information\n" \
	"\n" \
	"This program is Free Software licensed under GNU GPLv3.\n"

#define O_HELPTEXT \
	"nimped is a modal text editor, and each mode has some associated key\n" \
	"binds. In order to effectively use the editor, you will need to learn\n" \
	"these (when the editor is opened, it starts in base mode):\n" \
	"\n" \
	"Base mode:\n" \
	"\n" \
	"\th          Move left one character\n" \
	"\tl          Move right one character\n" \
	"\tj          Move down one line\n" \
	"\tk          Move up one line\n" \
	"\ta          Move to the start of the current line\n" \
	"\te          Move to the end of the current line\n" \
	"\tf          Move forward one word\n" \
	"\tb          Move backward one word\n" \
	"\tC-x C-c    Quit nimped\n" \
	"\tC-n        Create a new scratch frame\n" \
	"\tC-f        Create a frame from a file\n" \
	"\tC-k        Kill the current frame\n" \
	"\tC-s        Save the contents of the current frame to a file\n" \
	"\tn          Goto the next frame\n" \
	"\tp          Goto the previous frame\n" \
	"\tm          Set the current frame as master\n" \
	"\tu          Undo the last changes made to a frame\n" \
	"\t/          Search the frame forwards for literal text\n" \
	"\t?          Search the frame backwards for literal text\n" \
	"\tc          Copy the current line\n" \
	"\td          Cut the current line\n" \
	"\tq c        Copy a given number of lines\n" \
	"\tq d        Cut a given number of lines\n" \
	"\tv          Paste the clipboard contents\n" \
	"\tz          Center the frame around the cursor\n" \
	"\tg          Goto a given line\n" \
	"\tF3         Start recording a macro\n" \
	"\tF4         Stop recording / execute a macro\n" \
	"\ti          Enter write mode\n" \
	"\tC-h        Display a help menu\n" \
	"\n" \
	"Write mode:\n" \
	"\n" \
	"\tC-g        Quit write mode\n" \
	"\tC-d        Delete the character in front of the cursor\n" \
	"\tBACKSPC    Delete the Character behind the cursor (handling smart parens)\n" \
	"\tM-BACKSPC  Delete the word behind the cursor\n" \
	"\tRET        Insert a new line (handling indentation and smart parens)\n" \
	"\tTAB        Insert a tab character or equivalent spaces (as configured)" \
	"\t(          Insert a smart left-paren\n" \
	"\t[          Insert a smart left-bracket\n" \
	"\t{          Insert a smart left-brace\n" \
	"\t\"          Insert a smart double-quote\n" \
	"\n" \
	"Prompt mode:\n" \
	"\n" \
	"\tC-g        Quit prompt with failure\n" \
	"\ty          (Confirmation prompts) quit prompt with success\n" \
	"\tn          (Confirmation prompts) quit prompt with failure\n" \
	"\tRET        (Text prompts) quit prompt with success\n" \
	"\tC-f        (Text prompts) move right one character\n" \
	"\tC-b        (Text prompts) move left one character\n" \
	"\tC-a        (Text prompts) move to the start of the prompt\n" \
	"\tC-e        (Text prompts) move to the end of the prompt\n" \
	"\tM-f        (Text prompts) move forward one word\n" \
	"\tM-b        (Text prompts) move backward one word\n" \
	"\tC-d        (Text prompts) delete the character in front of the cursor\n" \
	"\tBACKSPC    (Text prompts) delete the character behind the cursor (handling smart parens)\n" \
	"\tM-BACKSPC  (Text prompts) delete the word behind the cursor\n" \
	"\t(          (Text prompts) insert a smart left-paren\n" \
	"\t[          (Text prompts) insert a smart left-bracket\n" \
	"\t{          (Text prompts) insert a smart left-brace\n" \
	"\t\"          (Text prompts) insert a smart double-quote\n"

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
	O_CCMODE,
	
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
extern e_char_t o_bexit[];
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
extern e_char_t o_bpyes[];
extern e_char_t o_bpno[];
extern e_char_t o_bhelp[];
extern e_char_t o_btab[];

typedef struct o_opts
{
	// layout options.
	u32 masternum, masterdenom;
	u32 lgutter, rgutter;
	u32 margin1, margin2;
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
	
	// editing options.
	bool tabspaces;
	
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
