#include "bmp.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lcdctl.h"

void bmp_display(struct LCD *lcd, const char *file, size_t r, size_t c) {
  if (lcd == NULL || lcd->addr == NULL) {
    fprintf(stderr, "LCD is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  int bmp_fd = open(file, O_RDONLY);
  if (bmp_fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // clang-format off
  uint32_t offset;
  int bmp_H, bmp_W;
  uint16_t depth;
  // https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
  lseek(bmp_fd, 0x0A, SEEK_SET); read(bmp_fd, &offset, 4);
  lseek(bmp_fd, 0x12, SEEK_SET); read(bmp_fd, &bmp_W, 4);
  lseek(bmp_fd, 0x16, SEEK_SET); read(bmp_fd, &bmp_H, 4);
  lseek(bmp_fd, 0x1C, SEEK_SET); read(bmp_fd, &depth, 2);
  if (depth != 32 && depth != 24) {
    fprintf(stderr, "Warning: Unsupported Bitmap, depth = %u\n", depth);
    return;
  }
  // https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
  const size_t row_size = (abs(bmp_W) * depth + 31) / 32 * 4;
  const size_t bmp_length = row_size * abs(bmp_W);
  char *color = (char *)(malloc(bmp_length * sizeof(char)));
  lseek(bmp_fd, offset, SEEK_SET); read(bmp_fd, color, bmp_length);
  close(bmp_fd);

  printf("DEBUG: offset = %zu\n", offset);
  printf("DEBUG: bmp_H = %d, bmp_W = %d, depth = %u\n", bmp_H, bmp_W, depth);
  printf("DEBUG: row_size = %zu, bmp_length = %zu\n", row_size, bmp_length);
  // clang-format on
  lcd->clear(lcd);
  size_t idx = 0;
  for (int i = 0; i < abs(bmp_H); i++) {
    for (int j = 0; j < abs(bmp_W); j++) {
      uint32_t A, R, G, B;
      B = color[idx++];
      G = color[idx++];
      R = color[idx++];
      A = (depth == 32 ? color[idx++] : 0);
      lcd->draw(lcd, (bmp_H < 0 ? i : SCREEN_H - i - 1) + r, (bmp_W > 0 ? j : SCREEN_W - j - 1) + c,
                (A << 24) | (R << 16) | (G << 8) | B);
    }
    while (idx % row_size != 0) idx++;
  }
  free(color);
}
