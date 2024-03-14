#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "fonts.h"
#include "lcdctl.h"

void rect_fill(struct LCD *lcd, size_t r_offset, size_t c_offset, size_t H, size_t W, uint32_t color) {
  for (size_t i = 0; i < H; i++) {
    if (i + r_offset >= SCREEN_H) continue;
    for (size_t j = 0; j < W; j++) {
      if (j + c_offset >= SCREEN_W) continue;
      lcd->draw(lcd, i + r_offset, j + c_offset, color);
    }
  }
}

void font_display(struct LCD *lcd, char ascii, size_t r_offset, size_t c_offset, uint32_t color) {
  const int idx = (int)ascii - 32;
  if (idx < 0 || idx >= 95) {
    fprintf(stderr, "Warning: unsupported font\n");
    return;
  }

  for (int r = 0; r < 24; r++) {
    for (int c = 0; c < 16; c++) {
      if (ASCII_Table[idx][r] >> c & 1) {
        lcd->draw(lcd, r_offset + r, c_offset + c, color);
      }
    }
  }
}

int main(void) {
  struct LCD lcd;
  LCD_constructor(&lcd);
  lcd.clear(&lcd);

  // int idx = 0;
  // for (int i = 0; i < SCREEN_H; i += 24) {
  //   for (int j = 0; j < SCREEN_W; j += 16) {
  //     if (idx == 95) continue;
  //     font_display(&lcd, idx + 32, i, j, 0xffffff);
  //     idx += 1;
  //   }
  // }

  rect_fill(&lcd, 0, 0, 480, 150, 0x00FF0000);
  rect_fill(&lcd, 0, 150, 480, 500, 0x0000FF00);
  rect_fill(&lcd, 0, 650, 480, 150, 0x000000FF);

  const size_t margin = 10;
  {
    const size_t card = 64;
    rect_fill(&lcd, margin * 1 + card * 0, margin, card, 150 - margin * 2, 0x00FFFFFF);
    rect_fill(&lcd, margin * 3 + card * 1, margin, card, 150 - margin * 2, 0x00FFFFFF);
    rect_fill(&lcd, margin * 5 + card * 2, margin, card, 150 - margin * 2, 0x00FFFFFF);
    rect_fill(&lcd, margin * 7 + card * 3, margin, card, 150 - margin * 2, 0x00FFFFFF);
  }
  {
    const size_t card = 220;
    rect_fill(&lcd, margin * 1 + card * 0, 150 + margin, card, 230, 0x00FFFFFF);
    rect_fill(&lcd, margin * 3 + card * 1, 150 + margin, card, 230, 0x00FFFFFF);
    rect_fill(&lcd, margin * 1 + card * 0, 150 + margin * 3 + card * 1, card, 230, 0x00FFFFFF);
    rect_fill(&lcd, margin * 3 + card * 1, 150 + margin * 3 + card * 1, card, 230, 0x00FFFFFF);
  }
  {
    const size_t card = 64;
    rect_fill(&lcd, margin, 650 + margin, card, 150 - margin * 2, 0x00FFFFFF);
    rect_fill(&lcd, 480 - margin - card, 650 + margin, card, 150 - margin * 2, 0x00FFFFFF);
    rect_fill(&lcd, 480 - margin * 2 - card * 3, 650 + margin, card * 2, 150 - margin * 2, 0x00FFFFFF);
  }

  LCD_destructor(&lcd);
  exit(EXIT_SUCCESS);
}
