#include "key_event_t.h"

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
