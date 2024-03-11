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
  int s_x, s_y, start = 1;
  while (1) {
    int status = read(self->fd, &ev, sizeof(struct input_event));
    if (status == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    printf("Debug: input event: type = %u, code = %u, value = %zu, \n", ev.type, ev.code, ev.value);
    if (ev.type == EV_ABS && ev.code == ABS_X) {
      self->x = ev.value;
    }
    if (ev.type == EV_ABS && ev.code == ABS_Y) {
      self->y = ev.value;
    }
    if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
      if (ev.value == 0 && start == 0) {
        // TODO: priority for operations
        if (self->x >= s_x + 50) {
          return RIGHT;
        }
        if (s_x >= self->x + 50) {
          return LEFT;
        }
        if (self->y >= s_y + 50) {
          return UP;
        }
        if (s_y >= self->y + 50) {
          return DOWN;
        }
        return TAP;
      }
      if (ev.value == 1 && start == 1) {
        start = 0;
        s_x = self->x;
        s_y = self->y;
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
