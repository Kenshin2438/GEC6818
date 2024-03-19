#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdint.h>

#include "lcdctl.h"

void font_display(struct LCD* lcd, char ascii, size_t r_offset, size_t c_offset, uint32_t text_color,
                  uint32_t back_color);

void number_display(struct LCD* lcd, uint32_t num, size_t r_offset /* right-bottom*/,
                    size_t c_offset /* right-bottom*/, uint32_t text_color, uint32_t back_color);

void string_display(struct LCD* lcd, char str[], int len, size_t r_offset, size_t c_offset,
                    uint32_t text_color, uint32_t back_color);

#endif  // !UTILITY_H_
