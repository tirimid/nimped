// SPDX-License-Identifier: GPL-3.0-or-later

#define E_REPLACEMENT 0xfffd
#define E_EMDASH 0x2014

typedef struct e_char
{
	u32 codepoint;
	char enc[4]; // need 4 bytes to cover unicode range.
} e_char_t;

e_char_t e_read(void);
e_char_t e_fread(FILE *fp);
e_char_t e_fromcodepoint(u32 codepoint);
e_char_t *e_fromstr(OUT usize *len, char const *s);
char *e_tostr(e_char_t const *data, usize len);
usize e_frommem(OUT e_char_t *ch, u8 const *p);
i32 e_putch(e_char_t ch);
i32 e_fputch(e_char_t ch, FILE *fp);
bool e_isprint(e_char_t ch);
bool e_isspace(e_char_t ch);
bool e_isalpha(e_char_t ch);
bool e_isdigit(e_char_t ch);
bool e_isalnum(e_char_t ch);
