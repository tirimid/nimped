// SPDX-License-Identifier: GPL-3.0-or-later

void i_unbind(void);
i32 i_bind(e_char_t const *bind, void (*fn)(void));
void i_organize(void);
e_char_t i_readrawkey(void);
e_char_t i_readkey(void);
void i_recmacro(void);
void i_stoprecmacro(void);
bool i_isrecmacro(void);
void i_execmacro(void);
