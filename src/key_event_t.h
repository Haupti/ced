#ifdef __WIN32__
#include <windows.h>
#endif

#ifndef KEY_EVENT_T_HEADER
#define KEY_EVENT_T_HEADER

typedef struct key_event {
  int is_key_down;
  unsigned short repeat_count;
  char key_value;
  unsigned long ctrl_key_state;
} key_event_t;

#ifdef __WIN32__
key_event_t to_key_event_t(INPUT_RECORD event) {
  key_event_t result = {
      .is_key_down = event.Event.KeyEvent.bKeyDown,
      .repeat_count = event.Event.KeyEvent.wRepeatCount,
      .key_value = event.Event.KeyEvent.uChar.AsciiChar,
      .ctrl_key_state = event.Event.KeyEvent.dwControlKeyState,
  };
  return result;
}
#endif
#endif
