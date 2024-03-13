#include "screen_touch.h"

#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static enum MOVE get_move(struct TouchInfo *self) {
  if (self == NULL) {
    fprintf(stderr, "TouchInfo is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  struct input_event ev;
  int s_x, s_y;  // start point coordinate
  while (1) {
    int status = read(self->fd, &ev, sizeof(struct input_event));
    if (status == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (ev.type == EV_ABS) {
      if (ev.code == ABS_X) self->x = ev.value * 800 / 1024;
      if (ev.code == ABS_Y) self->y = ev.value * 480 / 600;
    } else if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
      if (ev.value == 1) {
        s_x = self->x;
        s_y = self->y;
      }
      if (ev.value == 0) {
        const int delta = 50;
        if (self->x > s_x && self->x - s_x >= delta) return RIGHT;
        if (s_x > self->x && s_x - self->x >= delta) return LEFT;
        if (s_y > self->y && s_y - self->y >= delta) return UP;
        if (self->y > s_y && self->y - s_y >= delta) return DOWN;
        return TAP;
      }
    }
  }
  return NONE;
}

void Touch_Init(struct TouchInfo *self) {
  if (self == NULL) {
    fprintf(stderr, "TouchInfo is uninitialized\n");
    exit(EXIT_FAILURE);
  }

  self->fd = open(TOUCH_DEV_PATH, O_RDONLY);
  if (self->fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  self->get_move = get_move;
}

void Touch_Deinit(struct TouchInfo *self) {
  if (self == NULL || self->fd == -1) {
    fprintf(stderr, "TouchInfo double free\n");
    exit(EXIT_FAILURE);
  }

  close(self->fd);
  self->fd = -1;
}
