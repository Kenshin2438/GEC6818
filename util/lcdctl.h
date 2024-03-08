#ifndef LCDCTL_H_
#define LCDCTL_H_

#include <stddef.h>
#include <stdint.h>

#define SCREEN_H 480
#define SCREEN_W 800
#define SCREEN_BYTES (SCREEN_H * SCREEN_W * sizeof(uint32_t))
#define LCD_DEV_PATH "/dev/fb0"

struct LCD {
  int fd;
  uint32_t *addr;

  void (*clear)(struct LCD *self);
  void (*draw)(struct LCD *self, size_t r, size_t c, uint32_t color);
};

void LCD_constructor(struct LCD *self);
void LCD_destructor(struct LCD *self);

#endif /* lcdctl.h */
