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

bool 
check_mult_overflow(size_t n1, size_t n2)
{
	// should in theory work
	size_t div1 = SIZE_MAX / n1;
	if (div1 < n2)
	{
		return true;
	}
	return false;
}

void *
reallocarr(void *ptr, size_t nmemb, size_t size)
{
	if (check_mult_overflow(nmemb, size))
	{
		errno = ENOMEM;
		return NULL;
	}
	return realloc(ptr, nmemb * size);
}
