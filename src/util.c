// SPDX-License-Identifier: GPL-3.0-or-later

void
showinfo(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512] = "info: ";
	vsnprintf(&msg[6], sizeof(msg) - 6, fmt, args);
	
	if (w_state.running)
	{
		r_setbarstr(msg);
	}
	else
	{
		fprintf(stderr, "%s\n", msg);
	}
	
	va_end(args);
}

void
showerr(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512] = "err: ";
	vsnprintf(&msg[5], sizeof(msg) - 5, fmt, args);
	
	if (w_state.running)
	{
		r_setbarstr(msg);
	}
	else
	{
		fprintf(stderr, "%s\n", msg);
	}
	
	va_end(args);
}

u64
fileid(char const *path, bool deref)
{
	int (*statfn[])(char const *restrict, struct stat *restrict) =
	{
		lstat,
		stat
	};
	
	struct stat stat;
	if (statfn[deref](path, &stat))
	{
		return 1;
	}
	
	return stat.st_ino;
}

bool
ispathsame(char const *patha, char const *pathb)
{
	return fileid(patha, true) == fileid(pathb, true);
}

char const *
fileext(char const *path)
{
	char const *s = strrchr(path, '.');
	return s ? s + 1 : "";
}

void *
hreallocarray(void *ptr, usize nmemb, usize size)
{
	if (!nmemb)
	{
		return realloc(ptr, 1);
	}
	
	// check for overflow, as reallocarray() would.
	volatile usize mul = nmemb * size;
	if (mul / nmemb != size)
	{
		errno = ENOMEM;
		return NULL;
	}
	
	return realloc(ptr, nmemb * size);
}

void *
hmemcpy(void *dst, void const *src, usize n)
{
	if (!n)
	{
		return dst;
	}
	
	return memcpy(dst, src, n);
}

void *
hmemmove(void *dst, void const *src, usize n)
{
	if (!n)
	{
		return dst;
	}
	
	return memmove(dst, src, n);
}

u64
unixus(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (u64)tv.tv_sec * 1000000 + (u64)tv.tv_usec;
}

void
begintimer(OUT u64 *timer)
{
	*timer = unixus();
}

void
endtimer(u64 timer, char const *name)
{
	u64 d = unixus() - timer;
	fprintf(stderr, "profile: %s: %llu\n", name, (unsigned long long)d);
}
