#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "bmp.h"
#include "lcdctl.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Plz, input a bmp image's path.\n");
    exit(EXIT_FAILURE);
  }

  struct LCD lcd;
  LCD_constructor(&lcd);

  bmp_display(&lcd, argv[1], 200, 200);

  exit(EXIT_SUCCESS);
}
