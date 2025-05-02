// SPDX-License-Identifier: GPL-3.0-or-later

void
showerr(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512];
	vsnprintf(msg, sizeof(msg), fmt, args);
	
	fprintf(stderr, "err: %s\n", msg);
	
	va_end(args);
}
