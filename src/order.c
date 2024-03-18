#include "order.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "fonts.h"
#include "utility.h"

struct Item {
  char name[32];
  char path[32];
  uint32_t number;
  uint32_t price;
};

struct Item cate[4][4];
uint32_t total = 0;

enum CategoryType { XIANG = 0, CHUAN, YUE, WATER };
static enum CategoryType cur = XIANG;

static void init_order(void) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      sprintf(cate[i][j].path, "./menu-pic/%d-%d.bmp", i + 1, j + 1);
      cate[i][j].number = 0;
    }
  }

  strcpy(cate[XIANG][0].name, "剁椒鱼头");
  cate[XIANG][0].price = 48;
  strcpy(cate[XIANG][1].name, "辣椒炒肉");
  cate[XIANG][1].price = 28;
  strcpy(cate[XIANG][2].name, "农家一碗香");
  cate[XIANG][2].price = 28;
  strcpy(cate[XIANG][3].name, "小炒黄牛肉");
  cate[XIANG][3].price = 48;

  strcpy(cate[CHUAN][0].name, "宫保鸡丁");
  cate[CHUAN][0].price = 28;
  strcpy(cate[CHUAN][1].name, "麻婆豆腐");
  cate[CHUAN][1].price = 25;
  strcpy(cate[CHUAN][2].name, "毛血旺");
  cate[CHUAN][2].price = 38;
  strcpy(cate[CHUAN][3].name, "鱼香肉丝");
  cate[CHUAN][3].price = 25;

  strcpy(cate[YUE][0].name, "白切鸡");
  cate[YUE][0].price = 53;
  strcpy(cate[YUE][1].name, "蜜汁叉烧");
  cate[YUE][1].price = 48;
  strcpy(cate[YUE][2].name, "蒜香排骨");
  cate[YUE][2].price = 42;
  strcpy(cate[YUE][3].name, "冰糖燕窝");
  cate[YUE][3].price = 29;

  strcpy(cate[WATER][0].name, "可乐");
  cate[WATER][0].price = 5;
  strcpy(cate[WATER][1].name, "雪碧");
  cate[WATER][1].price = 5;
  strcpy(cate[WATER][2].name, "啤酒");
  cate[WATER][2].price = 8;
  strcpy(cate[WATER][3].name, "茅台");
  cate[WATER][3].price = 188;
}

static void page_flush(struct LCD* lcd) {  // clang-format off
  { // region part-A
    for (int i = 0; i < 480; i++) for (int j = 0; j < 150; j++) { lcd->draw(lcd, i, j, 0x1B813E); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (ZH_XIANG[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 + 10 * 1 + 64 * 0, pos % 128 + 10, cur == XIANG ? 0x000000 : 0xFFFFFF); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (ZH_CHUAN[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 + 10 * 3 + 64 * 1, pos % 128 + 10, cur == CHUAN ? 0x000000 : 0xFFFFFF); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (  ZH_YUE[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 + 10 * 5 + 64 * 2, pos % 128 + 10, cur ==   YUE ? 0x000000 : 0xFFFFFF); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (ZH_WATER[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 + 10 * 7 + 64 * 3, pos % 128 + 10, cur == WATER ? 0x000000 : 0xFFFFFF); }
  } // endregion part-A
  { // region part-B
    for (int i = 0; i < 480; i++) for (int j = 150; j < 650; j++) { lcd->draw(lcd, i, j, 0xFFFFFF); }
    bmp_display(lcd, cate[cur][0].path,       10, 150 + 10, 0);
    number_display(lcd, cate[cur][0].number, 240 - 5, 400 - 5 - 16, 0x66BAB7);

    bmp_display(lcd, cate[cur][1].path,       10, 400 + 10, 0);
    number_display(lcd, cate[cur][1].number, 240 - 5, 650 - 5 - 16, 0x66BAB7);

    bmp_display(lcd, cate[cur][2].path, 240 + 10, 150 + 10, 0);
    number_display(lcd, cate[cur][2].number, 480 - 5, 400 - 5 - 16, 0x66BAB7);

    bmp_display(lcd, cate[cur][3].path, 240 + 10, 400 + 10, 0);
    number_display(lcd, cate[cur][3].number, 480 - 5, 650 - 5 - 16, 0x66BAB7);

    total = 0;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) total += cate[i][j].number * cate[i][j].price;
  } // endregion part-B
  { // region part-C
    for (int i = 0; i < 480; i++) for (int j = 650; j < 800; j++) { lcd->draw(lcd, i, j, 0x1B813E); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (ZH_FANHUI[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 +  10, pos % 128 + 10 + 650, 0xFFFFFF); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (ZH_XIADAN[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 + 406, pos % 128 + 10 + 650, 0xFFFFFF); }
    for (int _ = 0, pos = 0; _ < 1024; _++) for (int b = 7; b >= 0; b--, pos++) if (  ZH_HEJI[_] >> b & 1)
    { lcd->draw(lcd, pos / 128 + 234, pos % 128 + 10 + 650, 0xFFFFFF); }
    number_display(lcd, total, 480 - 10 - 64 - 10 - 32, 800 - 10, 0xFFFFFF);
    font_display(lcd, '$', 480 - 10 - 64 - 10 - 32 - 24, 800 - 10 - 16 * 6, 0xFFFFFF);
  } // endregion part-C
}  // clang-format on

enum ORDER_STATUS { ORDER_BREAK, ORDER_FLUSH, ORDER_PUSH, ORDER_NONE };
static enum ORDER_STATUS check_status(struct TouchInfo* sTouch) {  // clang-format off
  enum MOVE type = NONE;
  while ((type = sTouch->get_move(sTouch)) != TAP) continue;
  if (sTouch->x >= 10 && sTouch->x <= 10 + 130) {
    if (sTouch->y >=  10 && sTouch->y <=  10 + 64) { cur = XIANG; return ORDER_FLUSH; }
    if (sTouch->y >=  94 && sTouch->y <=  94 + 64) { cur = CHUAN; return ORDER_FLUSH; }
    if (sTouch->y >= 178 && sTouch->y <= 178 + 64) { cur =   YUE; return ORDER_FLUSH; }
    if (sTouch->y >= 262 && sTouch->y <= 262 + 64) { cur = WATER; return ORDER_FLUSH; }
  } else if (sTouch->x >= 10 + 650 && sTouch->x <= 10 + 650 + 130) {
    if (sTouch->y >= 10 && sTouch->y <= 10 + 64) return ORDER_BREAK;
    if (sTouch->y >= 480 - 10 - 64 && sTouch->y <= 480 - 10) return ORDER_PUSH;
  } else {
    if (sTouch->y >= 10 && sTouch->y <= 10 + 210 && sTouch->x >= 150 + 10 && sTouch->x <= 150 + 10 + (230 / 2))
      cate[cur][0].number += 1;
    if (sTouch->y >= 10 && sTouch->y <= 10 + 210 && sTouch->x >= 400 + 10 && sTouch->x <= 400 + 10 + (230 / 2))
      cate[cur][1].number += 1;
    if (sTouch->y >= 240 + 10 && sTouch->y <= 240 + 10 + 210 && sTouch->x >= 150 + 10 && sTouch->x <= 150 + 10 + (230 / 2))
      cate[cur][2].number += 1;
    if (sTouch->y >= 240 + 10 && sTouch->y <= 240 + 10 + 210 && sTouch->x >= 400 + 10 && sTouch->x <= 400 + 10 + (230 / 2))
      cate[cur][3].number += 1;

    if (sTouch->y >= 10 && sTouch->y <= 10 + 210 && sTouch->x >= 150 + 10 + (230 / 2) && sTouch->x <= 150 + 10 + 230)
      if (cate[cur][0].number >= 1) { cate[cur][0].number -= 1; }
    if (sTouch->y >= 10 && sTouch->y <= 10 + 210 && sTouch->x >= 400 + 10 + (230 / 2) && sTouch->x <= 400 + 10 + 230)
      if (cate[cur][1].number >= 1) { cate[cur][1].number -= 1; }
    if (sTouch->y >= 240 + 10 && sTouch->y <= 240 + 10 + 210 && sTouch->x >= 150 + 10 + (230 / 2) && sTouch->x <= 150 + 10 + 230)
      if (cate[cur][2].number >= 1) { cate[cur][2].number -= 1; }
    if (sTouch->y >= 240 + 10 && sTouch->y <= 240 + 10 + 210 && sTouch->x >= 400 + 10 + (230 / 2) && sTouch->x <= 400 + 10 + 230)
      if (cate[cur][3].number >= 1) { cate[cur][3].number -= 1; }
  }
  return ORDER_NONE;
}  // clang-format on

static void order_push_socket(void) {
  if (total == 0) return;
}

void order(struct LCD* lcd, struct TouchInfo* sTouch) {
  init_order();
  while (1) {
    page_flush(lcd);
    enum ORDER_STATUS opt;
    while ((opt = check_status(sTouch)) != ORDER_NONE) {
      if (opt == ORDER_BREAK) return;
      if (opt == ORDER_FLUSH || opt == ORDER_PUSH) break;
    }
    if (opt == ORDER_PUSH) {
      order_push_socket();
      return;
    }
  }
}
