#include "key_event_t.h"
#include <stdio.h>

#define ESC "\x1b"
#define CSI ESC "["

#define CTRL_C 3
#define CHAR_A 65
#define CHAR_z 122
#define SHIFT 16;
#define CTRL 8;

typedef struct cursor_pos {
  int x;
  int y;
} cursor_pos_t;

cursor_pos_t cpos = {.x = 1, .y = 1};

void vt_clear_screen() { printf(CSI "2J"); }
void vt_set_cursor_pos(cursor_pos_t pos) { printf(CSI "%d;%df", pos.y, pos.x); }
void vt_erase_to_EOL(){ printf(CSI "0K"); }

void report(char *msg) {
  vt_set_cursor_pos((cursor_pos_t){1, 50});
  vt_erase_to_EOL();
  printf("%s", msg);
  vt_set_cursor_pos(cpos);
}

void vt_init() {
  cpos = (cursor_pos_t){.x = 1, .y = 1};
  vt_clear_screen();
  vt_set_cursor_pos(cpos);
}

void handle_key_event(key_event_t event, void (*exit_callback)(void *)) {
  if (event.key_value == CTRL_C) {
    exit_callback(NULL);
  } else if (event.key_value >= CHAR_A && event.key_value <= CHAR_z) {
    cpos.x++;
    printf("%c", event.key_value);
  } else {
    char string[128];
    sprintf(string, "[%d,%lu]", event.key_value,
            event.ctrl_key_state);
    report(string);
  }
}
