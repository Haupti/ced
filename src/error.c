#include <stdio.h>
#include <stdlib.h>

void err_msg(char *msg) {
  printf("ERROR: %s\n", msg);
}

void err_exit(char *msg) {
  err_msg(msg);
  exit(EXIT_FAILURE);
}

