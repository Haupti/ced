#include "cursor.h"
#include "vt.h"

cursor_pos_t cpos = {.x = 1, .y = 1};

void x_inc_cursor() { cpos.x++; }
void y_inc_cursor() { cpos.y++; }
void x_dec_cursor() {
  if (!(cpos.x == 0)) {
    cpos.x--;
  }
}
void y_dec_cursor() {
  if (!(cpos.y == 0)) {
    cpos.y--;
  }
}

void move_cursor_up(int n) {
  y_inc_cursor();
  vt_set_cursor_pos(cpos.x, cpos.y);
}
void move_cursor_down(int n) {
  y_dec_cursor();
  vt_set_cursor_pos(cpos.x, cpos.y);
}
void move_cursor_left(int n) {
  x_dec_cursor();
  vt_set_cursor_pos(cpos.x, cpos.y);
}
void move_cursor_right(int n) {
  x_inc_cursor();
  vt_set_cursor_pos(cpos.x, cpos.y);
}
cursor_pos_t set_cursor(cursor_pos_t pos) {
  // save a copy of current cursor position
  cursor_pos_t prev_pos = {.x = cpos.x, .y = cpos.y};

  // set new cursor position
  cpos.x = pos.x;
  cpos.y = pos.y;
  vt_set_cursor_pos(cpos.x, cpos.y);

  return prev_pos;
}

cursor_pos_t create_cursor_pos(int x, int y) { return (cursor_pos_t){.x = x, .y = y}; }
