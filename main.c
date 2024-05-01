#include "src/error.h"
#include "src/key_event_t.h"
#include "src/key_handler.h"
#include "src/mirror.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __WIN32__
#include <windows.h>
#endif
#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif

#define bool_t int
#define true 1
#define false 0

#ifdef __WIN32__
struct reset_handle_params {
  void *stdin_handle;
  unsigned long console_mode_in;
  void *stdout_handle;
  unsigned long console_mode_out;
};
#endif

#ifdef __WIN32__
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
#endif

#ifdef __WIN32__
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
   * initialize
   */
  terminal_init();
  init_display_mirror();

  /*
   * handling of input
   */
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
#endif

#ifdef __linux__
void reset_handles_and_exit(struct termios attr_in) {
  if (!tcsetattr(STDIN_FILENO, TCSADRAIN, &attr_in)) {
    err_msg("could not get set handle attributes");
  }
  exit(EXIT_SUCCESS);
}
#endif

void exit_callback(bool_t *exit_flag) {
  static bool_t *flag_ref = NULL;
  if (exit_flag != NULL) {
    flag_ref = exit_flag;
  }
  if (exit_flag == NULL) {
    *flag_ref = true;
  }
}

#ifdef __linux__
int main() {
  struct termios origin_attr_in;
  struct termios origin_attr_out;
  struct termios attr;
  cfmakeraw(&attr);

  if (tcgetattr(STDIN_FILENO, &origin_attr_in) == -1) {
    err_msg("could not get input handle attributes");
  }
  if (tcgetattr(STDOUT_FILENO, &origin_attr_out) == -1) {
    err_msg("could not get output handle attributes");
  }
  if (tcsetattr(STDIN_FILENO, TCSADRAIN, &attr) == -1) {
    err_msg("could not get set handle attributes");
  }

  size_t bufin_size = 4;
  char bufin[4];
  int read_count = 0;

  bool_t close_on_next_occasion = false;
  exit_callback(&close_on_next_occasion);
  while (!close_on_next_occasion) {
    read_count = read(STDIN_FILENO, bufin, bufin_size);
    if (read_count < 0) {
      err_msg("could not read from input handle");
    }
    printf("GOT: '%s'\n", bufin);
    if (bufin[0] == 'q') {
      close_on_next_occasion = true;
    }
  }
}
#endif
