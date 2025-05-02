// SPDX-License-Identifier: GPL-3.0-or-later

// standard library.
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// system dependencies.
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// project headers.
#include "util.h"
#include "o_options.h"
#include "a_args.h"
#include "e_encoding.h"
#include "r_render.h"
#include "f_frame.h"
#include "w_wnd.h"

// project source.
#include "a_args.c"
#include "e_encoding.c"
#include "f_frame.c"
#include "o_options.c"
#include "r_render.c"
#include "util.c"
#include "w_wnd.c"

int
main(int argc, char const *argv[])
{
	if (a_parse(argc, argv))
	{
		return 1;
	}
	
	if (o_parse())
	{
		return 1;
	}
	
	if (r_init())
	{
		return 1;
	}
	atexit(r_quit);
	
	if (w_init())
	{
		return 1;
	}
	
	return 0;
}
