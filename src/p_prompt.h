// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum p_rc
{
	P_NONE = 0,
	P_FAIL,
	P_SUCCESS
} p_rc_t;

typedef struct p_prompt
{
	e_char_t data[O_MAXPROMPTLEN];
	u32 len, start;
	i32 csr;
	p_rc_t rc;
} p_prompt_t;

extern p_prompt_t p_prompt;

void p_begin(e_char_t const *prompt, u32 len);
void p_beginstr(char const *prompt);
void p_end(void);
void p_writech(e_char_t ch, u32 pos);
void p_write(e_char_t const *data, u32 pos, u32 n);
void p_erase(u32 lb, u32 ub);
void p_render(void);
e_char_t *p_getdata(OUT usize *len);
char *p_getdatastr(void);
void p_pathcomplete(void);
