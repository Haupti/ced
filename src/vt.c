#include "vt.h"
#include <stdio.h>

void vt_clear_screen() { printf(CSI "2J"); }
void vt_set_cursor_pos(int x, int y) { printf(CSI "%d;%df", y, x); }
void vt_erase_to_EOL() { printf(CSI "0K"); }
void vt_move_cursor_up(int n) { printf(CSI "%dA", n); }
void vt_move_cursor_down(int n) { printf(CSI "%dB", n); }
void vt_move_cursor_right(int n) { printf(CSI "%dC", n); }
void vt_move_cursor_left(int n) { printf(CSI "%dD", n); }
