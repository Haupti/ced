#include "mirror.h"

#ifndef PERSISTENCE_IO_HEADER
#define PERSISTENCE_IO_HEADER

int save_to_file(display_mirror data);
void set_file_name(char * new_filename);
display_mirror * create_read_from_file();
#endif
