#include <stdio.h>
#include <stdlib.h>

void err_exit(char *msg) {
  printf("ERROR: %s\n", msg);
  exit(EXIT_FAILURE);
}
