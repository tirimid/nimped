// SPDX-License-Identifier: GPL-3.0-or-later

#define E_INVALIDCODEPOINT 0xffffffff
#define E_REPLACECODEPOINT 0xfffd

typedef struct e_char
{
	u32 codepoint;
	char enc[4]; // need 4 bytes to cover unicode range.
} e_char;

e_char e_read(void);
e_char e_fread(FILE *fp);
e_char e_fromcodepoint(u32 codepoint);
e_char *e_fromstr(OUT usize *len, char const *s);
usize e_frommem(OUT e_char *ch, u8 const *p);
i32 e_putch(e_char ch);
i32 e_fputch(e_char ch, FILE *fp);
bool e_isprint(e_char ch);
