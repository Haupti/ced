#include <stdio.h>
#ifndef VIRTUAL_TERMINAL_CONTROLS_HEADER
#define VIRTUAL_TERMINAL_CONTROLS_HEADER

/*
 * control keys
 */
#define ESC "\x1b"
#define CSI ESC "["

typedef struct cursor_pos {
  int x;
  int y;
} cursor_pos_t;

cursor_pos_t cpos = {.x = 1, .y = 1};

void vt_clear_screen();
void vt_set_cursor_pos(cursor_pos_t pos);
void vt_erase_to_EOL();
void vt_move_cursor_up();
void vt_move_cursor_down();
void vt_move_cursor_left();
void vt_move_cursor_right();

#endif
