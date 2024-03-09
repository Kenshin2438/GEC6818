#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)

int main(int argc, char *argv[]) {
  printf("options in [0-2]: ");
  int option;
  scanf("%d", &option);

  if (option < 0 || option > 2) handle_error("Invaild option\n");

  int fd_lcd = open("/dev/fb0", O_RDWR);
  if (fd_lcd == -1) handle_error("DEBUG: open /dev/fb0 failed\n");

  const int H = 480, W = 800;
  const size_t fb0_size = H * W * sizeof(int);
  char *addr = mmap(NULL, fb0_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd, 0);
  if (addr == MAP_FAILED) handle_error("mmap");

#define idx(i, j) (i * W + j)

  switch (option) {
    case 0: {
      const int COLOR[] = {0x000000, 0xDD0000, 0xFFCC00};
      for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
          ((int *)addr)[idx(i, j)] = COLOR[i / 160];
        }
      }
    } break;

    case 1: {
      const int COLOR[2][2] = {{0x000000, 0xDD0000}, {0xFFCC00, 0xAABBCC}};
      for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
          ((int *)addr)[idx(i, j)] = COLOR[i / 240][j / 400];
        }
      }
    } break;

    case 2: {
      const int R = 100, x = 240, y = 400;
      const int COLOR[2] = {0xFFFFFF, 0xFF0000};
      for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
          int id = (int)((i - x) * (i - x) + (j - y) * (j - y) <= R * R);
          ((int *)addr)[idx(i, j)] = COLOR[id];
        }
      }
    } break;
  }

  munmap(addr, fb0_size);
  close(fd_lcd);
  exit(EXIT_SUCCESS);
}
