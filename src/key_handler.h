#include "key_event_t.h"

#ifndef KEY_HANDLER_HEADER
#define KEY_HANDLER_HEADER

void vt_init();
void handle_key_event(key_event_t event, void (*exit_callback)(void *));

#endif
