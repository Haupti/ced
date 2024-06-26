#include "mirror.h"
#include "error.h"
#include "io.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


char *create_data(size_t size) {
  char *data = calloc(size, sizeof(*data));
  return data;
}

char *resize_data(char *data, size_t old_size, size_t new_size) {
  char *ptr = realloc(data, sizeof(*ptr) * new_size);
  if (ptr == NULL) {
    // TODO save current mirror to swp file
    err_exit("could not resize line, not enought memory");
  }
  char *end_of_data = ptr + old_size;
  memset((void *)(end_of_data), 0, (size_t)new_size - old_size);
  return ptr;
}

display_mirror *mirror;

void mirror_save_to_file() { save_to_file(*mirror); }

void mirror_set_char(int x, int y, char c) {
  if (y >= mirror->len) {
    /*
     * calcuate how often you have to double the number of rows
     * so y is within the display mirror
     */
    int exponent = 1;
    int found = 0;
    while (!found) {
      if (y < (mirror->len * pow(2, exponent))) {
        found = 1;
        break;
      }
      exponent++;
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
    mirror->len = new_len;
  }
  if (x >= mirror->rows[y].len) {
    row_t target_row = mirror->rows[y];
    int exponent = 1;
    int found = 0;
    while (!found) {
      if (x < (target_row.len * pow(2, exponent))) {
        found = 1;
        break;
      }
      exponent++;
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

  mirror->rows[y].data[x] = c;
}

void init_display_mirror() {
  row_t *rows = malloc(sizeof(*rows) * INIT_ROWS);
  for (int i = 0; i < INIT_ROWS; i++) {
    rows[i] = (row_t){.data = create_data(INIT_COLS), .len = INIT_COLS};
  }
  mirror = malloc(sizeof(display_mirror));
  *mirror = (display_mirror){.len = INIT_ROWS, .rows = rows};
}
