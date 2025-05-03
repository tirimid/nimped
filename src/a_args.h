// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct a_argdata
{
	char const *files[O_MAXFILES];
	usize nfiles;
	bool create;
} a_argdata;

extern a_argdata a_args;

i32 a_parse(i32 argc, char const *argv[]);
