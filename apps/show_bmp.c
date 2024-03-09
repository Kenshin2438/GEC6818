#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "lcdctl.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Plz, input a bmp image's path.\n");
    exit(EXIT_FAILURE);
  }

  struct LCD lcd;
  LCD_constructor(&lcd);

  int bmp_fd = open(argv[1], O_RDONLY);
  if (bmp_fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // clang-format off
  size_t offset;
  u_int16_t depth;
  int bmp_H, bmp_W;

  lseek(bmp_fd, 0x0A, SEEK_SET); read(bmp_fd, &offset, 4U);
  lseek(bmp_fd, 0x12, SEEK_SET); read(bmp_fd, &bmp_W, 4U);
  lseek(bmp_fd, 0x16, SEEK_SET); read(bmp_fd, &bmp_H, 4U);
  lseek(bmp_fd, 0x1C, SEEK_SET); read(bmp_fd, &depth, 2U);

  const size_t bmp_length = abs(bmp_H) * abs(bmp_W) * depth / 8;
  char *color = (char *)(malloc(bmp_length * sizeof(char)));

  lseek(bmp_fd, offset, SEEK_SET); read(bmp_fd, color, bmp_length);
  close(bmp_fd);

  printf("DEBUG: offset = %zu\n", offset);
  printf("DEBUG: bmp_H = %d, bmp_W = %d, depth = %u\n", bmp_H, bmp_W, depth);
  printf("DEBUG: bmp_length = %zu\n", bmp_length);
  // clang-format on

  lcd.clear(&lcd);
  size_t data_idx = 0;
  for (int i = 0; i < abs(bmp_H); i++) {
    for (int j = 0; j < abs(bmp_W); j++) {
      u_int32_t R, G, B, A;
      B = color[data_idx++];
      G = color[data_idx++];
      R = color[data_idx++];
      A = (depth == 32 ? color[data_idx++] : 0);
      if (i >= SCREEN_H || j >= SCREEN_W) {
        continue;  // out of range
      }
      lcd.draw(&lcd, bmp_H < 0 ? i : SCREEN_H - i - 1, bmp_W > 0 ? j : SCREEN_W - j - 1,
               (A << 24) | (R << 16) | (G << 8) | B);
    }
  }
  free(color);
  LCD_destructor(&lcd);
  exit(EXIT_SUCCESS);
}
