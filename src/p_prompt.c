// SPDX-License-Identifier: GPL-3.0-or-later

p_prompt_t p_prompt =
{
	.csr = -1
};

void
p_begin(e_char_t const *prompt, u32 len)
{
	len = len > O_MAXPROMPTLEN ? O_MAXPROMPTLEN : len;
	
	p_prompt.len = len;
	p_prompt.start = len;
	memcpy(&p_prompt.data[0], prompt, sizeof(e_char_t) * len);
	
	p_prompt.csr = len;
	p_prompt.rc = P_NONE;
}

void
p_beginstr(char const *prompt)
{
	usize len;
	e_char_t *eprompt = e_fromstr(&len, prompt);
	p_begin(eprompt, len);
	free(eprompt);
}

void
p_end(void)
{
	p_prompt.csr = -1;
}

void
p_writech(e_char_t ch, u32 pos)
{
	p_write(&ch, pos, 1);
}

void
p_write(e_char_t const *data, u32 pos, u32 n)
{
	if (p_prompt.len + n > O_MAXPROMPTLEN)
	{
		n = O_MAXPROMPTLEN - p_prompt.len;
	}
	
	memmove(
		&p_prompt.data[pos + n],
		&p_prompt.data[pos],
		sizeof(e_char_t) * (p_prompt.len - pos)
	);
	memcpy(&p_prompt.data[pos], data, sizeof(e_char_t) * n);
	p_prompt.len += n;
}

void
p_erase(u32 lb, u32 ub)
{
	memmove(&p_prompt.data[lb], &p_prompt.data[ub], sizeof(e_char_t) * (p_prompt.len - ub));
	p_prompt.len -= ub - lb;
}

void
p_render(void)
{
	r_setbar(p_prompt.data, p_prompt.len);
}

e_char_t *
p_getdata(OUT usize *len)
{
	*len = p_prompt.len - p_prompt.start;
	e_char_t *data = calloc(*len, sizeof(e_char_t));
	memcpy(data, &p_prompt.data[p_prompt.start], sizeof(e_char_t) * *len);
	return data;
}

char *
p_getdatastr(void)
{
	return e_tostr(&p_prompt.data[p_prompt.start], p_prompt.len - p_prompt.start);
}

void
p_pathcomplete(void)
{
	if ((u32)p_prompt.csr < p_prompt.len)
	{
		return;
	}
	
	char *promptdata = p_getdatastr();
	char path[PATH_MAX + 1] = {0};
	strncpy(path, promptdata, PATH_MAX);
	free(promptdata);
	
	usize len = strlen(path);
	if (len >= PATH_MAX)
	{
		return;
	}
	
	char name[PATH_MAX + 1] = {0}, dir[PATH_MAX + 1] = {0};
	strncpy(dir, path, PATH_MAX);
	char *firstslash = strchr(dir, '/'), *lastslash = strrchr(dir, '/');
	
	char *firstch = dir;
	while (*firstch && isspace(*firstch))
	{
		++firstch;
	}
	
	if (!lastslash)
	{
		dir[0] = '.';
		dir[1] = 0;
		strncpy(name, path, sizeof(name));
	}
	else if (firstch && firstch == firstslash && firstslash == lastslash)
	{
		strncpy(name, &lastslash[1], PATH_MAX);
		lastslash[1] = 0;
	}
	else
	{
		strncpy(name, &lastslash[1], PATH_MAX);
		lastslash[0] = 0;
	}
	
	usize namelen = strlen(name);
	
	DIR *dp = opendir(dir);
	if (!dp)
	{
		return;
	}
	
	for (struct dirent *de = readdir(dp); de; de = readdir(dp))
	{
		if (strncasecmp(name, de->d_name, namelen)
			|| (de->d_type != DT_DIR && de->d_type != DT_REG)
			|| !strcmp(".", de->d_name)
			|| !strcmp("..", de->d_name))
		{
			continue;
		}
		
		char newpath[PATH_MAX + 1] = {0};
		strcpy(newpath, dir);
		
		// only add '/' if not root.
		if (firstch && firstch != lastslash)
		{
			strcat(newpath, "/");
		}
		
		strcat(newpath, de->d_name);
		if (de->d_type == DT_DIR)
		{
			strcat(newpath, "/");
		}
		
		usize enewlen;
		e_char_t *enewpath = e_fromstr(&enewlen, newpath);
		enewlen = enewlen + p_prompt.start < O_MAXPROMPTLEN ? enewlen : O_MAXPROMPTLEN - p_prompt.start;
		memcpy(&p_prompt.data[p_prompt.start], enewpath, sizeof(e_char_t) * enewlen);
		free(enewpath);
		p_prompt.len = p_prompt.start + enewlen;
		p_prompt.csr = p_prompt.len;
		
		closedir(dp);
		return;
	}
	
	closedir(dp);
}

bool
p_iswritable(e_char_t ch)
{
	return e_isprint(ch) && ch.codepoint != E_REPLACEMENT;
}
