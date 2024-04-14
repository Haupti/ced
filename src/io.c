#include "error.h"
#include "mirror.h"
#include <stdio.h>

#define S(x) #x

int save_to_file(display_mirror data) {
  char *filename = "test.txt";

  FILE *fp = fopen(filename, "a");
  if (fp == NULL) {
    printf("cannot open file\n");
    return 0;
  }

  for (int y = 0; y < data.len; y++) {
    char line[data.rows[y].len];
    for (int x = 0; x < data.rows[y].len; x++) {
      if (data.rows[y].data[x] == 0) {
        line[x] = ' ';
      } else {
        line[x] = data.rows[y].data[x];
      }
    }
    fprintf(fp, "%s\n", line);
  }
  fclose(fp);
  return 1;
}
