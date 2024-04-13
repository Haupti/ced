#include "vt.h"
#include <stdio.h>

void vt_clear_screen() { printf(CSI "2J"); }
void vt_set_cursor_pos(cursor_pos_t pos) { printf(CSI "%d;%df", pos.y, pos.x); }
void vt_erase_to_EOL() { printf(CSI "0K"); }
void vt_move_cursor_up() {
  if (cpos.x == 0) {
    return;
  }
  cpos.x--;
  vt_set_cursor_pos(cpos);
}
void vt_move_cursor_down() {
  cpos.x++;
  vt_set_cursor_pos(cpos);
}
void vt_move_cursor_left() {
  if (cpos.y == 0) {
    return;
  }
  cpos.y--;
  vt_set_cursor_pos(cpos);
}
void vt_move_cursor_right() {
  cpos.y++;
  vt_set_cursor_pos(cpos);
}
