// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct i_bind
{
	e_char_t const *bind;
	usize len;
	void (*fn)(void);
} i_bind_t;

static int i_bindcmp(void const *lhs, void const *rhs);

static i_bind_t i_binds[O_MAXBINDS];
static usize i_nbinds;
static e_char_t i_curbind[O_MAXBINDLEN];
static usize i_curbindlen;

void
i_unbind(void)
{
	i_nbinds = 0;
	i_curbindlen = 0;
}

i32
i_bind(e_char_t const *bind, void (*fn)(void))
{
	if (i_nbinds >= O_MAXBINDS)
	{
		showerr("input: cannot register more than %u keybinds!", O_MAXBINDLEN);
		return 1;
	}
	
	usize len = 0;
	while (bind[len].codepoint)
	{
		++len;
	}
	
	for (usize i = 0; i < i_nbinds; ++i)
	{
		if (i_binds[i].len == len
			&& !memcmp(bind, i_binds[i].bind, sizeof(e_char_t) * len))
		{
			i_binds[i].fn = fn;
			return 0;
		}
	}
	
	i_binds[i_nbinds++] = (i_bind_t)
	{
		.bind = bind,
		.len = len,
		.fn = fn
	};
	
	return 0;
}

void
i_organize(void)
{
	qsort(i_binds, i_nbinds, sizeof(i_bind_t), i_bindcmp);
}

e_char_t
i_readrawkey(void)
{
	// TODO: implement macro system eventually.
	return e_read();
}

e_char_t
i_readkey(void)
{
	e_char_t k = i_readrawkey();
	if (!i_nbinds)
	{
		return k;
	}
	
	if (i_curbindlen < O_MAXBINDLEN)
	{
		i_curbind[i_curbindlen++] = k;
	}
	else
	{
		i_curbindlen = 0;
	}
	
	isize low = 0, high = i_nbinds - 1, mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		
		i_bind_t other =
		{
			.bind = i_curbind,
			.len = i_curbindlen
		};
		
		i32 cmp = i_bindcmp(&other, &i_binds[mid]);
		switch (cmp)
		{
		case -1:
			high = mid - 1;
			break;
		case 0:
			goto found;
		case 1:
			low = mid + 1;
			break;
		}
	}
	
	mid = -1;
found:
	if (mid != -1)
	{
		i_bind_t const *bind = &i_binds[mid];
		if (bind->len == i_curbindlen)
		{
			bind->fn();
			i_curbindlen = 0;
		}
		
		return e_fromcodepoint(E_REPLACEMENT);
	}
	
	i_curbindlen = 0;
	return k;
}

static int
i_bindcmp(void const *lhs, void const *rhs)
{
	i_bind_t const *lhsbind = lhs, *rhsbind = rhs;
	
	for (usize i = 0; i < lhsbind->len && i < rhsbind->len; ++i)
	{
		if (lhsbind->bind[i].codepoint > rhsbind->bind[i].codepoint)
		{
			return 1;
		}
		else if (lhsbind->bind[i].codepoint < rhsbind->bind[i].codepoint)
		{
			return -1;
		}
	}
	
	return 0;
}
