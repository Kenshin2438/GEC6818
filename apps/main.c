#include <stdint.h>
#include <stdlib.h>

#include "game2048.h"
#include "lcdctl.h"
#include "screen_touch.h"

// apps-source file
#include "album.h"
#include "order.h"

enum App { INVAILD, ALBUM, GAME2048, ORDER, EXIT };
static enum App choose(struct TouchInfo *sTouch) {
  enum MOVE type;
  while ((type = sTouch->get_move(sTouch)) != TAP) continue;

  // const size_t padding = 0;
  if (sTouch->x >= 000 && sTouch->x < 200) return ALBUM;
  if (sTouch->x >= 200 && sTouch->x < 400) return ORDER;
  if (sTouch->x >= 400 && sTouch->x < 600) return GAME2048;
  // if (sTouch->x >= 600 && sTouch->x < 800) return EXIT;

  return INVAILD;
}

void homepage(struct LCD *lcd) {
  static const uint32_t color[] = {
    0x00FEDFE1,
    0x00A5DEE4,
    0x008A6BBE,
    0x00A8497A,
  };
  for (int i = 0; i < SCREEN_H; i++) {
    for (int j = 0; j < SCREEN_W; j++) {
      lcd->draw(lcd, i, j, color[j / 200]);
    }
  }
}

int main(void) {
  struct LCD lcd;
  LCD_constructor(&lcd);
  lcd.clear(&lcd);

  struct TouchInfo sTouch;
  Touch_Init(&sTouch);

  while (1) {
    homepage(&lcd);
    enum App name;
    while ((name = choose(&sTouch)) == INVAILD) continue;
    if (name == ALBUM) album(&lcd, &sTouch);
    if (name == ORDER) order(&lcd, &sTouch);
    if (name == GAME2048) game2048(&lcd, &sTouch);

    if (name == EXIT) {
      lcd.clear(&lcd);
      break;
    }
  }

  LCD_destructor(&lcd);
  Touch_Deinit(&sTouch);

  exit(EXIT_SUCCESS);
}
