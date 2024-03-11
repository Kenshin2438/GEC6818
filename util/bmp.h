#ifndef BMP_H_
#define BMP_H_

#include "lcdctl.h"

void bmp_display(struct LCD* lcd, const char* file, size_t r, size_t c);

#endif  // !BMP_H_
