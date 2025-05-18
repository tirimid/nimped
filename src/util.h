// SPDX-License-Identifier: GPL-3.0-or-later

#define OUT

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ssize_t isize;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;

typedef float f32;
typedef double f64;

void showinfo(char const *fmt, ...);
void showerr(char const *fmt, ...);
u64 fileid(char const *path, bool deref);
bool ispathsame(char const *patha, char const *pathb);
char const *fileext(char const *path);
void *hreallocarray(void *ptr, usize nmemb, usize size);
void *hmemcpy(void *dst, void const *src, usize n);
void *hmemmove(void *dst, void const *src, usize n);
