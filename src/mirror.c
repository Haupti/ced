#include "mirror.h"
#include "error.h"
#include <math.h>
#include <stdlib.h>

#define INIT_COLS 80

char *create_data(size_t size) {
  char *data = malloc(sizeof(char) * size);
  return data;
}

display_mirror *mirror;

void set_char(int x, int y, char c) {
  if (y >= mirror->len) {
    /*
     * calcuate how often you have to double the number of rows
     * so y is within the display mirror
     */
    int exponent = 1;
    int found = 0;
    while (!found) {
      exponent++;
      if (y < (mirror->len * pow(2, exponent))) {
        found = 1;
      }
    }
    /*
     * allocate the required number of rows
     * and set the mirrors rows
     */
    int old_len = mirror->len;
    int new_len = old_len * pow(2, exponent);
    row_t *ptr = realloc(mirror->rows, sizeof(row_t) * new_len);
    if (ptr == NULL) {
      // TODO save current mirror to swp file
      err_exit("could not allocate all required memory to write the new "
               "character (column)");
    }
    mirror->rows = ptr;
    // allocate the rows
    for (int i = old_len; i < new_len; i++) {
      mirror->rows[i] =
          (row_t){.data = create_data(INIT_COLS), .len = INIT_COLS};
    }
  }
  row_t target_row = mirror->rows[y];
  if (x >= target_row.len) {
    int exponent = 0;
    int found = 0;
    while (!found) {
      exponent++;
      if (x < (target_row.len * pow(2, exponent))) {
        found = 1;
      }
    }
    int old_len = target_row.len;
    int new_len = pow(2, exponent) * old_len;
    char *ptr = realloc(target_row.data, sizeof(char) * new_len);
    if (ptr == NULL) {
      // TODO save current mirror to swp file
      err_exit("could not allocate all required memory to write the new "
               "character (line)");
    }
    target_row.data = ptr;
    target_row.len = new_len;
    mirror->rows[y] = target_row;
  }

  printf(" <%d,%d> ", mirror->rows[x].len, mirror->len);
  mirror->rows[y].data[x] = c;
}

void init_display_mirror() {
  int rows_count = 50;
  row_t *rows = malloc(sizeof(row_t) * rows_count);
  for (int i = 0; i < rows_count; i++) {
    rows[i] = (row_t){.data = create_data(INIT_COLS), .len = INIT_COLS};
  }
  mirror = malloc(sizeof(display_mirror));
  *mirror = (display_mirror){.len = rows_count, .rows = rows};
}
