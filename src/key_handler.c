#include "key_event_t.h"
#include <stdio.h>

#define ESC "\x1b"
#define CSI ESC "["

#define CTRL_C 3
#define CHAR_A 65
#define CHAR_Z 90
#define CHAR_a 97
#define CHAR_z 122
#define CHAR_c 99
#define SHIFT 16
#define CTRL 8
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

typedef struct cursor_pos {
  int x;
  int y;
} cursor_pos_t;

cursor_pos_t cpos = {.x = 1, .y = 1};

void vt_clear_screen() { printf(CSI "2J"); }
void vt_set_cursor_pos(cursor_pos_t pos) { printf(CSI "%d;%df", pos.y, pos.x); }
void vt_erase_to_EOL() { printf(CSI "0K"); }
void move_cursor_up() {
  cpos.x--;
  vt_set_cursor_pos(cpos);
}
void move_cursor_down() {
  cpos.x++;
  vt_set_cursor_pos(cpos);
}
void move_cursor_left() {
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
  vt_erase_to_EOL();
  printf("%d", event_count);
  if (event_count == 1) {
    key_event_t event = events[0];

    if (event.key == CTRL_C) {
      exit_callback(NULL);
      return;
    } else if ((event.key >= CHAR_A && event.key <= CHAR_Z) ||
               (event.key >= CHAR_a && event.key <= CHAR_z)) {
      cpos.x++;
      printf("%c", event.key);
      report_events(events, event_count);
      return;
    }

  } else if (event_count == 2) {
    key_event_t event_fst = events[0];
    key_event_t event_snd = events[1];
    if (event_fst.key == CTRL && event_snd.key == CHAR_c) {
      exit_callback(NULL);
      return;
    }
  }

  report_events(events, event_count);
}
