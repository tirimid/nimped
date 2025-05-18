// SPDX-License-Identifier: GPL-3.0-or-later

a_args_t a_args;

static void a_usage(char const *name);

i32
a_parse(i32 argc, char const *argv[])
{
	i32 ch;
	while (ch = getopt(argc, (char *const *)argv, "cho:"), ch != -1)
	{
		switch (ch)
		{
		case 'c':
			a_args.create = true;
			break;
		case 'h':
			a_usage(argv[0]);
			exit(0);
		case 'o':
			a_args.confdir = optarg;
			break;
		default:
			return 1;
		}
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
	
	for (usize i = 0; i < a_args.nfiles; ++i)
	{
		for (usize j = i + 1; j < a_args.nfiles; ++j)
		{
			if (ispathsame(a_args.files[i], a_args.files[j]))
			{
				showerr("args: cannot open the same file twice - %s!", a_args.files[i]);
				return 1;
			}
		}
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
		"\t-c      Create files if they don't exist\n"
		"\t-h      Display this help information\n"
		"\t-o dir  Use a different config directory\n",
		name
	);
}
