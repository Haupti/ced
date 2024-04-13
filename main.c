#include "src/key_event_t.h"
#include "src/key_handler.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __WIN32__
#include <windows.h>
#endif

void err_exit(char *msg) {
  printf("ERROR: %s\n", msg);
  exit(EXIT_FAILURE);
}

struct reset_handle_params {
  void *stdin_handle;
  unsigned long console_mode_in;
  void *stdout_handle;
  unsigned long console_mode_out;
};

void reset_handles_and_exit(struct reset_handle_params *given) {
  static struct reset_handle_params *local = NULL;
  if (given != NULL) {
    local = given;
    return;
  }
  if (local == NULL) {
    return;
  }
  if (!SetConsoleMode(local->stdin_handle, local->console_mode_in)) {
    err_exit("could not reset console input mode");
  }
  if (!SetConsoleMode(local->stdout_handle, local->console_mode_out)) {
    err_exit("could not reset console output mode");
  }
  exit(EXIT_SUCCESS);
}

int main(int args, char **argv) {

  size_t in_buffer_size = 4;

  void *stdin_handle = NULL;
  unsigned long read_count = 0;
  unsigned long console_mode_in = 0;
  INPUT_RECORD input_buffer[in_buffer_size];
  unsigned long original_console_mode_in = 0;

  /*
   * input setup
   */
  stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
  if (stdin_handle == INVALID_HANDLE_VALUE) {
    err_exit("could not get input handle");
  }

  if (!GetConsoleMode(stdin_handle, &original_console_mode_in)) {
    err_exit("could not get original input mode");
  }

  console_mode_in =
      ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT | !ENABLE_ECHO_INPUT;
  if (!SetConsoleMode(stdin_handle, console_mode_in)) {
    err_exit("could not set console input mode");
  }

  /*
   * output setup
   */
  void *stdout_handle = NULL;
  unsigned long original_console_mode_out = 0;
  unsigned long console_mode_out = 0;

  stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (stdout_handle == INVALID_HANDLE_VALUE) {
    err_exit("could not get output handle");
  }

  if (!GetConsoleMode(stdout_handle, &original_console_mode_out)) {
    err_exit("could not get original output mode");
  }

  console_mode_out =
      ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
  if (!SetConsoleMode(stdout_handle, console_mode_out)) {
    err_exit("could not set console output mode");
  }

  struct reset_handle_params reset_params = {
      .stdin_handle = stdin_handle,
      .console_mode_in = console_mode_in,
      .stdout_handle = stdout_handle,
      .console_mode_out = console_mode_out,
  };

  reset_handles_and_exit(&reset_params);
  void (*exit_callback)(void *) = (void (*)(void *)) & reset_handles_and_exit;

  /*
   * handling of input
   */
  vt_init();

  key_event_t events[in_buffer_size];
  int key_event_index = 0;
  while (1) {
    key_event_index = 0;
    if (!ReadConsoleInput(stdin_handle, input_buffer, in_buffer_size,
                          &read_count)) {
      err_exit("reading console input failed");
    }
    for (int i = 0; i < read_count; i++) {
      if (input_buffer[i].EventType == KEY_EVENT) {
        events[key_event_index] = to_key_event_t(input_buffer[i]);
        key_event_index++;
      }
    }
    handle_key_event(events, key_event_index, exit_callback);
  }

  return EXIT_SUCCESS;
}
