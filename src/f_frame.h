// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum f_flag
{
	F_UNSAVED = 0x1
} f_flag;

typedef enum f_histtype
{
	F_WRITE = 0,
	F_ERASE,
	F_BREAK
} f_histtype;

typedef union f_histentry
{
	struct
	{
		u8 type;
		u32 lb, ub;
	} write;
	
	struct
	{
		u8 type;
		u32 lb;
		e_char *data;
	} erase;
	
	u8 type;
} f_histentry;

typedef struct f_frame
{
	e_char *buf;
	u32 len, cap;
	char *src;
	u32 start, csr;
	u32 svcsrx;
	u64 flags;
	f_histentry *hist;
	u32 histlen, histcap;
} f_frame;

f_frame f_create(void);
f_frame f_fromstr(char const *s);
i32 f_fromfile(OUT f_frame *f, char const *file);
void f_destroy(f_frame *f);
void f_render(f_frame const *f, u32 x, u32 y, u32 w, u32 h, bool active);
i32 f_save(f_frame *f);
void f_writech(f_frame *f, e_char ch, u32 pos);
void f_write(f_frame *f, e_char *data, u32 pos, usize n);
void f_erase(f_frame *f, u32 lb, u32 ub);
void f_undo(f_frame *f);
void f_breakhist(f_frame *f);
void f_savecsr(f_frame *f);
void f_loadcsr(f_frame *f);
void f_compbounds(f_frame *f, u32 w, u32 h);
