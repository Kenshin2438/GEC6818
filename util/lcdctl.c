#include "lcdctl.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static void clear(struct LCD *self) {
  if (self == NULL || self->addr == NULL) {
    perror("LCD is uninitialized");
    exit(EXIT_FAILURE);
  }

  memset(self->addr, 0x00, SCREEN_BYTES);
}

static void draw(struct LCD *self, size_t r, size_t c, uint32_t color) {
  if (self == NULL || self->addr == NULL) {
    perror("LCD is uninitialized");
    exit(EXIT_FAILURE);
  }

  if (r >= SCREEN_H || c >= SCREEN_W) {
    fprintf(stderr, "lcd draw idx = (%zu, %zu)\n", r, c);
    exit(EXIT_FAILURE);
  }

  self->addr[r * SCREEN_W + c] = color;
}

void LCD_constructor(struct LCD *self) {
  if (self == NULL) {
    perror("LCD_Init self is NULL");
    exit(EXIT_FAILURE);
  }

  self->fd = open(LCD_DEV_PATH, O_RDWR);
  if (self->fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  self->addr = mmap(NULL, SCREEN_BYTES, PROT_WRITE | PROT_READ, MAP_SHARED, self->fd, 0);
  if (self->addr == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  self->clear = clear;
  self->draw = draw;
}

void LCD_destructor(struct LCD *self) {
  if (self != NULL) {
    close(self->fd);
    munmap(self->addr, SCREEN_BYTES);
    self = NULL;
  }
}
