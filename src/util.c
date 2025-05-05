// SPDX-License-Identifier: GPL-3.0-or-later

void
showerr(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512];
	vsnprintf(msg, sizeof(msg), fmt, args);
	
	if (w_state.running)
	{
		// TODO: show error message during editor execution.
	}
	else
	{
		fprintf(stderr, "err: %s\n", msg);
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
