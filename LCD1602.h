#pragma once

#include "pico/stdlib.h"

#define MAX_LINES 2
#define MAX_CHARS 16

void lcd_clear(void);

void lcd_set_cursor(int line, int position);

void lcd_string(const char *s);

void lcd_init();
