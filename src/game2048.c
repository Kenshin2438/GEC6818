#include "game2048.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
  int idx_array[16][2], cnt = 0;
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_N; j++) {
      if (board[i][j] == 0) {
        idx_array[cnt][0] = i;
        idx_array[cnt][1] = j;
        cnt++;
      }
    }
  }
  if (cnt == 0) return;
  int idx = rand() % cnt;
  board[idx_array[idx][0]][idx_array[idx][1]] = (rand() % 4 == 0 ? 4 : 2);
}

static void board_display(struct LCD* lcd) {
  {  // center
    const size_t delta = 80 + 4;
    const size_t pr = 240 - (80 + 4 + 80 + 2);
    const size_t pc = 400 - (80 + 4 + 80 + 2);
    for (int i = 0; i < BOARD_N; i++) {
      for (int j = 0; j < BOARD_N; j++) {
        if (board[i][j] == 0) {
          for (int r = 0; r < 80; r++) {
            for (int c = 0; c < 80; c++) {
              lcd->draw(lcd, r + pr + delta * i, c + pc + delta * j, 0xBDC0BA);
            }
          }
        } else {
          char filename[128];
          sprintf(filename, "picture/%d.bmp", board[i][j]);
          bmp_display(lcd, filename, SCREEN_H - (pr + delta * i) - 80, pc + delta * j, 0);
        }
      }
    }
  }
  {  // left
    number_display(lcd, score, 24, 16 * 8, 0xBDC0BA, 0x000000);
  }
  {  // right
    const char* BACK = "BACK";
    const size_t pr = 10;
    const size_t pc = SCREEN_W - 10 - 10 * 2 - 4 * 16;  // 706
    for (int i = 0; i < 24 + 10 * 2; i++) {
      for (int j = 0; j < 64 + 10 * 2; j++) {
        lcd->draw(lcd, pr + i, pc + j, 0xBDC0BA);
      }
    }
    for (int i = 0; i < 4; i++) {
      font_display(lcd, BACK[i], pr + 10, pc + 10 + 16 * i, 0x000000, 0xBDC0BA);
    }
  }
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
    for (int j = 0; j < BOARD_N / 2; j++) {
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
static void move_UP(void)    { rotate_left90(); move_LEFT(); rotate_left90(); rotate_left90(); rotate_left90(); }
static void move_RIGHT(void) { rotate_left90(); rotate_left90(); move_LEFT(); rotate_left90(); rotate_left90(); }
static void move_DOWN(void)  { rotate_left90(); rotate_left90(); rotate_left90(); move_LEFT(); rotate_left90(); }
// clang-format on

enum GAME_STATUS { OVER, BACK };
static enum GAME_STATUS game_start(struct LCD* lcd, struct TouchInfo* sTouch) {
  init_board();
  lcd->clear(lcd);
  board_display(lcd);

  while (game_is_over() == false) {
    enum MOVE type;
    while ((type = sTouch->get_move(sTouch)) == TAP) {
      if (sTouch->y >= 10 && sTouch->y <= 10 + 24 + 10 * 2 && sTouch->x >= 706 && sTouch->y <= 790) {
        return BACK;
      }
    }
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
  sleep(1);
  return OVER;
}

void game2048(struct LCD* lcd, struct TouchInfo* sTouch) {
  enum GAME_STATUS status;
  while ((status = game_start(lcd, sTouch)) == OVER) {
    enum MOVE type;
    while ((type = sTouch->get_move(sTouch)) == NONE) continue;
  }
}
