
#ifndef CURSOR_CONTROLS_HEADER
#define CURSOR_CONTROLS_HEADER

typedef struct cursor_pos {
  int x;
  int y;
} cursor_pos_t;

void x_inc_cursor();
void y_inc_cursor();
void x_dec_cursor();
void y_dec_cursor();
cursor_pos_t get_current_cursor_pos();
void move_cursor_up(int n);
void move_cursor_down(int n);
void move_cursor_left(int n);
void move_cursor_right(int n);

// returns cursor position before change
cursor_pos_t set_cursor(cursor_pos_t pos);

cursor_pos_t create_cursor_pos(int x, int y);

#endif
