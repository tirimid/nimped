// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct h_region
{
	u32 lb, ub;
	u8 fg, bg;
} h_region_t;

void h_find(OUT h_region_t *r, f_frame_t const *f, u32 from);
void h_findc(OUT h_region_t *r, f_frame_t const *f, u32 from);
void h_findsh(OUT h_region_t *r, f_frame_t const *f, u32 from);
void h_findjs(OUT h_region_t *r, f_frame_t const *f, u32 from);
void h_findcc(OUT h_region_t *r, f_frame_t const *f, u32 from);
