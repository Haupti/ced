#include <stdio.h>
#include <stdlib.h>

#ifdef __WIN32__
#include <windows.h>
#else
// TODO linux part
#endif

void err_exit(char *msg) {
  printf("ERROR: %s\n", msg);
  exit(EXIT_FAILURE);
}

char key_value(INPUT_RECORD event) {
  return event.Event.KeyEvent.uChar.AsciiChar;
}

int main(int args, char **argv) {

  void *stdin_handle;
  unsigned long read_count, console_mode;
  INPUT_RECORD input_buffer[128];
  unsigned long origian_console_mode;

  stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
  if (stdin_handle == INVALID_HANDLE_VALUE) {
    err_exit("could not get input handle");
  }

  if (!GetConsoleMode(stdin_handle, &origian_console_mode)) {
    err_exit("could not get original input mode");
  }

  console_mode =
      ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT | !ENABLE_ECHO_INPUT;
  if (!SetConsoleMode(stdin_handle, console_mode)) {
    err_exit("could not set console mode");
  }

  while (1) {
    if (!ReadConsoleInput(stdin_handle, input_buffer, 128, &read_count)) {
      err_exit("reading console input failed");
    }
    for (int i = 0; i < read_count; i++) {
      INPUT_RECORD event = input_buffer[i];
      switch (input_buffer[i].EventType) {
      case KEY_EVENT:
          if(event.Event.KeyEvent.bKeyDown){
            printf("Key event: %c (%d)\n", key_value(event), key_value(event));
          }
          if(key_value(event) == 'X'){
              printf("exit...");
              return EXIT_SUCCESS;
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
