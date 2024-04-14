
#ifndef DISPLAY_MIRROR_HEADER
#define DISPLAY_MIRROR_HEADER

typedef struct _row_t {
  int len;
  char *data;
} row_t;

typedef struct _file_mirror {
  int len;
  row_t *rows;
} display_mirror;

void init_display_mirror();
void mirror_set_char(int x, int y, char c);
void mirror_save_to_file();

#endif
