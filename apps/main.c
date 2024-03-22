#include <stdlib.h>

#include "bmp.h"
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

  const int width = SCREEN_W / 3;
  if (sTouch->x >= width * 0 && sTouch->x < width * 1) return ALBUM;
  if (sTouch->x >= width * 1 && sTouch->x < width * 2) return ORDER;
  if (sTouch->x >= width * 2 && sTouch->x < width * 3) return GAME2048;
  // if (sTouch->x >= 600 && sTouch->x < 800) return EXIT;

  return INVAILD;
}

void homepage(struct LCD *lcd) { bmp_display(lcd, "home.bmp", 0, 0, 1); }

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
