#include <pthread.h>
#include <stdlib.h>

#include "bmp.h"
#include "lcdctl.h"
#include "screen_touch.h"

static const char *files[] = {
    "assets/1.bmp", "assets/2.bmp", "assets/3.bmp", "assets/ba.bmp", "assets/fufu.bmp", "assets/genshin.bmp",
};

int main(int argc, char *argv[]) {
  struct LCD lcd;
  LCD_constructor(&lcd);
  lcd.clear(&lcd);

  struct TouchInfo sTouch;
  Touch_Init(&sTouch);

  const int N = sizeof(files) / sizeof(char *);

  while (1) {
    static size_t idx = 0;
    while (1) {
      enum MOVE type = sTouch.get_move(&sTouch);
      if (type == RIGHT || type == UP) {
        idx = (idx + 1) % N;
      }
      if (type == LEFT || type == DOWN) {
        idx = (idx - 1 + N) % N;
      }
      if (type == TAP) break;
      bmp_display(&lcd, files[idx], 0, 0);
    }
  }

  LCD_destructor(&lcd);
  Touch_Deinit(&sTouch);

  exit(EXIT_SUCCESS);
}
