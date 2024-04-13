#include "key_event_t.h"
#include <stdio.h>

#define ESC "\x1b"
#define CSI ESC "["

#define CTRL_C 3
#define CHAR_A 65
#define CHAR_B 66
#define CHAR_C 67
#define CHAR_D 68
#define CHAR_Z 90
#define CHAR_a 97
#define CHAR_z 122
#define CHAR_c 99
#define SHIFT 16
#define SPACE 92
#define CTRL 8
#define CTRL_BRACKET_SO 27 // bracket square open
#define BRACKET_SO 91      // bracket square open

typedef struct cursor_pos {
  int x;
  int y;
} cursor_pos_t;

cursor_pos_t cpos = {.x = 1, .y = 1};

void vt_clear_screen() { printf(CSI "2J"); }
void vt_set_cursor_pos(cursor_pos_t pos) { printf(CSI "%d;%df", pos.y, pos.x); }
void vt_erase_to_EOL() { printf(CSI "0K"); }
void move_cursor_up() {
  if (cpos.x == 0) {
    return;
  }
  cpos.x--;
  vt_set_cursor_pos(cpos);
}
void move_cursor_down() {
  cpos.x++;
  vt_set_cursor_pos(cpos);
}
void move_cursor_left() {
  if (cpos.y == 0) {
    return;
  }
  cpos.y--;
  vt_set_cursor_pos(cpos);
}
void move_cursor_right() {
  cpos.y++;
  vt_set_cursor_pos(cpos);
}

void vt_init() {
  cpos = (cursor_pos_t){.x = 1, .y = 1};
  vt_clear_screen();
  vt_set_cursor_pos(cpos);
}

void report_events(key_event_t *events, int event_count) {
  vt_set_cursor_pos((cursor_pos_t){1, 50});
  vt_erase_to_EOL();
  for (int i = 0; i < event_count; i++) {
    printf("[%d,%lu]", events[i].key, events[i].ctrl_key_state);
  }
  vt_set_cursor_pos(cpos);
}

void handle_key_event(key_event_t *events, int event_count,
                      void (*exit_callback)(void *)) {
  if (event_count == 1 && events[0].is_key_down) {
    key_event_t event = events[0];

    if (event.key == CTRL_C) {
      exit_callback(NULL);
      return;
    } else if ((event.key >= CHAR_A && event.key <= CHAR_Z) ||
               (event.key >= CHAR_a && event.key <= CHAR_z) ||
               event.key == SPACE) {
      cpos.x++;
      printf("%c", event.key);
      report_events(events, event_count);
      return;
    }

  } else if (event_count == 3) {
    key_event_t event_fst = events[0];
    key_event_t event_snd = events[1];
    key_event_t event_trd = events[2];
    if (!(event_fst.is_key_down && event_snd.is_key_down &&
          event_trd.is_key_down)) {
      return;
    }

    if (event_fst.key == CTRL_BRACKET_SO && event_snd.key == BRACKET_SO &&
        event_trd.key == CHAR_A) { // arrow left
      move_cursor_left();
    } else if (event_fst.key == CTRL_BRACKET_SO &&
               event_snd.key == BRACKET_SO &&
               event_trd.key == CHAR_B) { // arrow right
      move_cursor_right();
    } else if (event_fst.key == CTRL_BRACKET_SO &&
               event_snd.key == BRACKET_SO &&
               event_trd.key == CHAR_C) { // arrow down
      move_cursor_down();
    } else if (event_fst.key == CTRL_BRACKET_SO &&
               event_snd.key == BRACKET_SO &&
               event_trd.key == CHAR_D) { // arrow up
      move_cursor_up();
    }
  }

  report_events(events, event_count);
}
