// SPDX-License-Identifier: GPL-3.0-or-later

#define E_REPLACEMENT 0xfffd

typedef struct e_char
{
	u32 codepoint;
	char enc[4]; // need 4 bytes to cover unicode range.
} e_char_t;

e_char_t e_read(void);
e_char_t e_fread(FILE *fp);
e_char_t e_fromcodepoint(u32 codepoint);
e_char_t *e_fromstr(OUT usize *len, char const *s);
usize e_frommem(OUT e_char_t *ch, u8 const *p);
i32 e_putch(e_char_t ch);
i32 e_fputch(e_char_t ch, FILE *fp);
bool e_isprint(e_char_t ch);
