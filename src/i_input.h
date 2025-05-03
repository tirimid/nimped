// SPDX-License-Identifier: GPL-3.0-or-later

void i_unbind(void);
i32 i_bind(e_char const *bind, void (*fn)(void));
void i_organize(void);
e_char i_readrawkey(void);
e_char i_readkey(void);
