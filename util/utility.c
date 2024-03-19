#include "utility.h"

#include <stdint.h>
#include <stdio.h>

#include "fonts.h"

void font_display(struct LCD* lcd, char ascii, size_t r_offset, size_t c_offset, uint32_t text_color,
                  uint32_t back_color) {
  const int idx = (int)ascii - 32;
  if (idx < 0 || idx >= 95) {
    fprintf(stderr, "Warning: unsupported font\n");
    return;
  }

  for (int r = 0; r < 24; r++) {
    for (int c = 0; c < 16; c++) {
      if (ASCII_Table[idx][r] >> c & 1) {
        lcd->draw(lcd, r_offset + r, c_offset + c, text_color);
      } else {
        lcd->draw(lcd, r_offset + r, c_offset + c, back_color);
      }
    }
  }
}

void number_display(struct LCD* lcd, uint32_t num, size_t r_offset /* right-bottom*/,
                    size_t c_offset /* right-bottom*/, uint32_t text_color, uint32_t back_color) {
  uint32_t tmp = num, cnt = 0;
  do {
    int ret = tmp % 10;
    tmp /= 10, cnt += 1;
    font_display(lcd, ret + '0', r_offset - 24, c_offset - 16 * cnt, text_color, back_color);
  } while (tmp != 0);
}

void string_display(struct LCD* lcd, char str[], int len, size_t r_offset, size_t c_offset,
                    uint32_t text_color, uint32_t back_color) {
  for (int i = 0; i < len; i++) {
    font_display(lcd, str[i], r_offset, c_offset + 16 * i, text_color, back_color);
  }
}
