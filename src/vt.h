#include <stdio.h>
#include "cursor.h"
#ifndef VIRTUAL_TERMINAL_CONTROLS_HEADER
#define VIRTUAL_TERMINAL_CONTROLS_HEADER

/*
 * control keys
 */
#define ESC "\x1b"
#define CSI ESC "["


void vt_clear_screen();
void vt_set_cursor_pos(int x, int y);
void vt_erase_to_EOL();
void vt_move_cursor_up(int n);
void vt_move_cursor_down(int n);
void vt_move_cursor_left(int n);
void vt_move_cursor_right(int n);

#endif
