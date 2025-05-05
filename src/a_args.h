// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct a_args
{
	char const *files[O_MAXFILES];
	usize nfiles;
	bool create;
} a_args_t;

extern a_args_t a_args;

i32 a_parse(i32 argc, char const *argv[]);
