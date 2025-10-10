// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum f_flag
{
	F_UNSAVED = 0x1
} f_flag_t;

typedef enum f_histtype
{
	F_WRITE = 0,
	F_ERASE,
	F_BREAK
} f_histtype_t;

typedef union f_hist
{
	struct
	{
		u8 type;
		u32 lb, ub;
	} write;
	
	struct
	{
		u8 type;
		u32 lb, ub;
		e_char_t *data;
	} erase;
	
	u8 type;
} f_hist_t;

typedef struct f_frame
{
	e_char_t *buf;
	u32 len, cap;
	char *src;
	u32 start, csr;
	u32 svcsrx;
	u64 flags;
	f_hist_t *hist;
	u32 histlen, histcap;
} f_frame_t;

f_frame_t f_create(void);
f_frame_t f_fromstr(char const *s);
i32 f_fromfile(OUT f_frame_t *f, char const *file);
void f_destroy(f_frame_t *f);
void f_render(f_frame_t const *f, u32 x, u32 y, u32 w, u32 h, bool active);
i32 f_save(f_frame_t *f);
void f_writech(f_frame_t *f, e_char_t ch, u32 pos);
void f_write(f_frame_t *f, e_char_t const *data, u32 pos, usize n);
void f_erase(f_frame_t *f, u32 lb, u32 ub);
void f_undo(f_frame_t *f);
void f_breakhist(f_frame_t *f);
void f_savecsr(f_frame_t *f);
void f_loadcsr(f_frame_t *f);
void f_compbounds(f_frame_t *f, u32 w, u32 h);
u32 f_tabulate(f_frame_t *f, u32 at);
