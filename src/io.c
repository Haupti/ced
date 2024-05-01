#include "io.h"
#include "error.h"
#include "mirror.h"
#include <stdio.h>

#define S(x) #x
char *filename = "test.txt";
void set_file_name(char *new_filename) { filename = new_filename; }

int save_to_file(display_mirror data) {
  FILE *fp = fopen(filename, "a");
  if (fp == NULL) {
    err_msg("cannot open file\n");
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

typedef struct _file_read_result {
  int length;
  int elem_size;
  void *data;
} file_read_result;

file_read_result create_read_text_from_file() {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    err_exit("cannot read file");
  }
  int c = fseek(fp, 0, SEEK_END);
  if (c != 0) {
    err_exit("cannot seek in file");
  }
  int filesize = ftell(fp);
  char *buffer = calloc(filesize + 1, sizeof(*buffer));
  if (buffer == NULL) {
    err_exit("cold not hold file in memory");
  }
  int read_count = fread(buffer, sizeof(*buffer), filesize, fp);
  buffer[filesize] = '\0';

  fclose(fp);
  file_read_result result = {
      .length = read_count, .elem_size = sizeof(*buffer), .data = buffer};
  return result;
}

/*
display_mirror *create_read_from_file() {
  file_read_result res = create_read_text_from_file();
  char *text_raw = res.data;
  int length = res.length;
  row_t *rows = NULL;

  int current_len = 0;
  char *current_row = calloc(INIT_ROWS, sizeof(*current_row));
  for (int i = 0; i < length; i++) {
    if (text_raw[i] == '\r') {
      // fuck this
    } else if (text_raw[i] == '\n') {
      // TODO
      // create row_t from current_row and add to list of row_t's
      // free current_row and allocate new one (frees memory :)
    } else {
        // TODO
        // if length < INIT_ROWS, increase length of the string (*2 etc)
      current_row[i] = text_raw[i];
      current_len++;
    }
  }
  free(current_row);
}
*/
