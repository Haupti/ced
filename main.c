#include <stdio.h>
#include <stdlib.h>
#include "src/key_event_t.h"
#include "src/key_handler.h"

#ifdef __WIN32__
#include <windows.h>
#endif

void err_exit(char *msg) {
  printf("ERROR: %s\n", msg);
  exit(EXIT_FAILURE);
}

void restore_modes(void *stdin_handle, unsigned long console_mode_in,
                   void *stdout_handle, unsigned long console_mode_out) {
  if (!SetConsoleMode(stdin_handle, console_mode_in)) {
    err_exit("could not reset console input mode");
  }
  if (!SetConsoleMode(stdout_handle, console_mode_out)) {
    err_exit("could not reset console output mode");
  }
}

int main(int args, char **argv) {

  size_t in_buffer_size = 4;

  void *stdin_handle;
  unsigned long read_count;
  unsigned long console_mode_in;
  INPUT_RECORD input_buffer[in_buffer_size];
  unsigned long original_console_mode_in;

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
  void *stdout_handle;
  unsigned long original_console_mode_out;
  unsigned long console_mode_out;

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

  /*
   * handling of input
   */
  while (1) {
    if (!ReadConsoleInput(stdin_handle, input_buffer, in_buffer_size,
                          &read_count)) {
      err_exit("reading console input failed");
    }
    for (int i = 0; i < read_count; i++) {
      INPUT_RECORD event = input_buffer[i];
      switch (input_buffer[i].EventType) {
      case KEY_EVENT:
        if (event.Event.KeyEvent.uChar.AsciiChar == 'X') {
          printf("exit...");
          restore_modes(stdin_handle, original_console_mode_in, stdout_handle,
                        original_console_mode_out);
          return EXIT_SUCCESS;
        }
        if (event.Event.KeyEvent.bKeyDown) {
            handle_key_event(to_key_event_t(event));
        }
        break;
      default:
        printf("...");
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
