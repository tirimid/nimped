// SPDX-License-Identifier: GPL-3.0-or-later

// standard library.
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// system dependencies.
#include <dirent.h>
#include <pwd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

// project headers.
#include "util.h"
#include "e_encoding.h"
#include "o_options.h"
#include "a_args.h"
#include "i_input.h"
#include "r_render.h"
#include "f_frame.h"
#include "h_highlight.h"
#include "w_wnd.h"
#include "b_binds.h"
#include "p_prompt.h"

// project source.
#include "a_args.c"
#include "b_binds.c"
#include "e_encoding.c"
#include "f_frame.c"
#include "h_highlight.c"
#include "i_input.c"
#include "o_options.c"
#include "p_prompt.c"
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
	
	if (w_init())
	{
		r_quit(false);
		return 1;
	}
	
	w_loop();
	
	r_quit(true);
	return 0;
}
