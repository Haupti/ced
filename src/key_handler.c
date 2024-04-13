#include "cursor.h"
#include "key_event_t.h"
#include "vt.h"
#include <stdio.h>
/*
 * keys that are represented by one key-event
 */
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

/*
 * keys that are represented by three individual key-events
 */
enum key3 {
  ARROW_UP,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  NOT_RECOGNIZED,
};

typedef struct key3_event {
  enum key3 key;
  int is_key_down;
} key3_event;

key3_event parse_key3_event(key_event_t *events) {
  key_event_t event_fst = events[0];
  key_event_t event_snd = events[1];
  key_event_t event_trd = events[2];

  enum key3 key;
  int is_key_down = 0;
  if (event_fst.is_key_down && event_snd.is_key_down && event_trd.is_key_down) {
    is_key_down = 1;
  }

  if (event_fst.key == CTRL_BRACKET_SO && event_snd.key == BRACKET_SO &&
      event_trd.key == CHAR_A) {
    key = ARROW_DOWN;
  } else if (event_fst.key == CTRL_BRACKET_SO && event_snd.key == BRACKET_SO &&
             event_trd.key == CHAR_B) {
    key = ARROW_UP;
  } else if (event_fst.key == CTRL_BRACKET_SO && event_snd.key == BRACKET_SO &&
             event_trd.key == CHAR_C) {
    key = ARROW_RIGHT;
  } else if (event_fst.key == CTRL_BRACKET_SO && event_snd.key == BRACKET_SO &&
             event_trd.key == CHAR_D) {
    key = ARROW_LEFT;
  } else {
    key = NOT_RECOGNIZED;
  }
  return (key3_event){.is_key_down = is_key_down, .key = key};
}

void terminal_init() {
  set_cursor(create_cursor_pos(1, 1));
  vt_clear_screen();
}

void report_events(key_event_t *events, int event_count) {
  cursor_pos_t saved = set_cursor(create_cursor_pos(1, 50));
  vt_erase_to_EOL();
  for (int i = 0; i < event_count; i++) {
    printf("[%d,%lu]", events[i].key, events[i].ctrl_key_state);
  }
  set_cursor(saved);
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
      x_inc_cursor();
      printf("%c", event.key);
      report_events(events, event_count);
      return;
    }

  } else if (event_count == 3) {
    key3_event key_event = parse_key3_event(events);

    if (!key_event.is_key_down) {
      return;
    }

    switch (key_event.key) {
    case ARROW_UP:
      move_cursor_up(1);
      break;
    case ARROW_DOWN:
      move_cursor_down(1);
      break;
    case ARROW_LEFT:
      move_cursor_left(1);
      break;
    case ARROW_RIGHT:
      move_cursor_right(1);
      break;
    default:
      break;
    }
  }

  report_events(events, event_count);
}
