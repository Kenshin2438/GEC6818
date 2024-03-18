#include "game2048.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bmp.h"
#include "lcdctl.h"
#include "screen_touch.h"
#include "utility.h"

#define BOARD_N 4  // sizeof game board
static uint32_t board[BOARD_N][BOARD_N];
uint64_t score;

static bool game_is_over(void) {
  for (size_t i = 0; i < BOARD_N; i++) {
    for (size_t j = 0; j < BOARD_N; j++) {
      if (board[i][j] == 2048) return true;
    }
  }

  for (size_t i = 0; i < BOARD_N; i++) {
    for (size_t j = 0; j < BOARD_N; j++) {
      if (board[i][j] == 0) return false;
      if (i > 0 && board[i][j] == board[i - 1][j]) return false;
      if (j > 0 && board[i][j] == board[i][j - 1]) return false;
    }
  }

  return true;
}

static void random_fill(void) {
  bool has_zero = false;
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_N; j++) {
      if (board[i][j] == 0) has_zero = true;
    }
  }
  if (!has_zero) return;
  bool filled = false;
  do {
    int x = rand() % BOARD_N;
    int y = rand() % BOARD_N;
    if (board[x][y] != 0) continue;
    board[x][y] = (rand() % 3 <= 1 ? 2 : 4);
  } while (!filled);
}

static void init_board(void) {
  for (size_t i = 0; i < BOARD_N; i++) {
    for (size_t j = 0; j < BOARD_N; j++) {
      board[i][j] = 0;
    }
  }
  score = 0;
  srand(time(NULL));

  random_fill();
  random_fill();
}

static void rotate_left90(void) {
  for (int i = 0; i < BOARD_N / 2; i++) {
    for (int j = i; j < BOARD_N - i - 1; j++) {
      uint32_t tmp = board[i][j];
      board[i][j] = board[j][BOARD_N - i - 1];
      board[j][BOARD_N - i - 1] = board[BOARD_N - i - 1][BOARD_N - j - 1];
      board[BOARD_N - i - 1][BOARD_N - j - 1] = board[BOARD_N - j - 1][i];
      board[BOARD_N - j - 1][i] = tmp;
    }
  }
}

static void move_row(int idx) {
  for (int i = 0, j = 0; i < BOARD_N; i++) {
    if (board[idx][i] != 0) {  // board[idx][j] ^= board[idx][i] ^= board[idx][j] ^= board[idx][i];
      uint32_t temp = board[idx][i];
      board[idx][i] = board[idx][j];
      board[idx][j] = temp;
      j++;
    }
  }
  for (int i = 0; i + 1 < BOARD_N; i++) {
    if (board[idx][i] == board[idx][i + 1]) {
      board[idx][i] += board[idx][i + 1];
      score += board[idx][i];
      board[idx][i + 1] = 0;
    }
  }
  for (int i = 0, j = 0; i < BOARD_N; i++) {
    if (board[idx][i] != 0) {  // board[idx][j] ^= board[idx][i] ^= board[idx][j] ^= board[idx][i];
      uint32_t temp = board[idx][i];
      board[idx][i] = board[idx][j];
      board[idx][j] = temp;
      j++;
    }
  }
}

// clang-format off
static void move_LEFT(void)  { for (int i = 0; i < BOARD_N; i++) move_row(i); }
static void move_UP(void)    { rotate_left90(); rotate_left90(); rotate_left90(); move_LEFT(); rotate_left90(); }
static void move_RIGHT(void) { rotate_left90(); rotate_left90(); move_LEFT(); rotate_left90(); rotate_left90(); }
static void move_DOWN(void)  { rotate_left90(); move_LEFT(); rotate_left90(); rotate_left90(); rotate_left90(); }
// clang-format on

void board_display(struct LCD* lcd) {
  const size_t delta = 80 + 4;
  int pr = 240 - (80 + 4 + 80 + 2);
  int pc = 400 - (80 + 4 + 80 + 2);
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_N; j++) {
      if (board[i][j] != 0) {
        for (int r = 0; r < 80; r++) {
          for (int c = 0; c < 80; c++) {
            lcd->draw(lcd, r + pr, c + pc, 0xBDC0BA);
          }
        }
      } else {
        char filename[128];
        sprintf(filename, "picture/%d.bmp", board[i][j]);
        bmp_display(lcd, filename, pr, pc, 0);
      }
      pc += delta;
    }
    pr += delta;
  }
  number_display(lcd, score, 24, 16 * 8, 0xBDC0BA);
}

void game2048(struct LCD* lcd, struct TouchInfo* sTouch) {
  init_board();
  lcd->clear(lcd);
  while (game_is_over() == false) {
    enum MOVE type;
    while ((type = sTouch->get_move(sTouch)) == TAP) continue;
    switch (type) {
      case UP: {
        move_UP();
      } break;
      case RIGHT: {
        move_RIGHT();
      } break;
      case DOWN: {
        move_DOWN();
      } break;
      case LEFT: {
        move_LEFT();
      } break;
      default:
        break;
    }
    random_fill();
    board_display(lcd);
  }

  lcd->clear(lcd);
  bmp_display(lcd, "picture/game_over.bmp", 0, 0, 1);
}
