#include "album.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bmp.h"

static size_t bmp_count(void) {
  FILE *fp;
  fp = popen("find assets/*.bmp | wc -l", "r");
  if (fp == NULL) {
    perror("popen");
    exit(EXIT_FAILURE);
  }
  char buf[128];
  fgets(buf, sizeof(buf), fp);
  pclose(fp);

  size_t N;
  sscanf(buf, "%u", &N);
  return N;
}

static void bmp_nth(struct LCD *lcd, size_t idx /** 0-indexed */) {
  char cmd[128];
  FILE *fp;
  sprintf(cmd, "find assets/*.bmp | sed -n %up", idx + 1);
  fp = popen(cmd, "r");
  if (fp == NULL) {
    perror("popen");
    exit(EXIT_FAILURE);
  }
  char buf[128];
  fgets(buf, sizeof(buf), fp);
  pclose(fp);

  sscanf(buf, "%s", buf);
  lcd->clear(lcd);
  bmp_display(lcd, buf, 0, 0, 1);
}

void album(struct LCD *lcd, struct TouchInfo *sTouch) {
  size_t N = bmp_count();

  if (N == 0) {
    fprintf(stderr, "No bmp file exists\n");
    lcd->clear(lcd);
  }

  static size_t idx = 0;
  idx %= N;

  bmp_nth(lcd, idx);
  while (1) {
    enum MOVE type = sTouch->get_move(sTouch);
    if (type == TAP || type == UP) continue;
    if (type == DOWN) break;
    if (type == RIGHT) {
      idx = (idx + 1) % N;
    }
    if (type == LEFT) {
      idx = (idx + N - 1) % N;
    }
    bmp_nth(lcd, idx);
  }
}
