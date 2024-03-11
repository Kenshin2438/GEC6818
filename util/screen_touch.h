#ifndef SCREEN_TOUCH_H_
#define SCREEN_TOUCH_H_

#include <linux/input.h>

#define TOUCH_DEV_PATH "/dev/input/event0"

enum MOVE { NONE, UP, RIGHT, DOWN, LEFT, TAP };
struct TouchInfo {
  int fd;
  int x, y;

  enum MOVE (*get_move)(struct TouchInfo* slef);
};

void Touch_Init(struct TouchInfo* self);
void Touch_Deinit(struct TouchInfo* self);

#endif  // !SCREEN_TOUCH_H_
