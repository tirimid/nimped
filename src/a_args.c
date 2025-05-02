// SPDX-License-Identifier: GPL-3.0-or-later

a_argdata a_args;

static void a_usage(char const *name);

i32
a_parse(i32 argc, char const *argv[])
{
	i32 ch;
	while (ch = getopt(argc, (char *const *)argv, "chr"), ch != -1)
	{
		switch (ch)
		{
		case 'c':
			a_args.create = true;
			break;
		case 'h':
			a_usage(argv[0]);
			exit(0);
		case 'r':
			a_args.readonly = true;
			break;
		default:
			return 1;
		}
	}
	
	if (a_args.create && a_args.readonly)
	{
		showerr("args: cannot specify -c and -r at the same time!");
		return 1;
	}
	
	for (; optind < argc; ++optind)
	{
		if (a_args.nfiles >= O_MAXFILES)
		{
			showerr("args: cannot open more than %u files!", O_MAXFILES);
			return 1;
		}
		
		a_args.files[a_args.nfiles++] = argv[optind];
	}
	
	return 0;
}

static void
a_usage(char const *name)
{
	fprintf(
		stderr,
		"nimped - (n)ew and (imp)roved text (ed)itor\n"
		"\n"
		"Usage:\n"
		"\t%s [options] files\n"
		"\n"
		"Options:\n"
		"\t-c  Create files if they don't exist\n"
		"\t-h  Display this help information\n"
		"\t-r  Open files as read-only\n",
		name
	);
}
