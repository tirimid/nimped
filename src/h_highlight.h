// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct h_region
{
	u32 lb, ub;
	u8 fg, bg;
} h_region_t;

void h_initmodes(void);
bool h_find(OUT h_region_t *r, f_frame_t const *f, u32 from);
