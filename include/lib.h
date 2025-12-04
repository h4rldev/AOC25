#ifndef LIB_H
#define LIB_H

typedef enum {
  ERR_OK = 0,
  ERR_MEM = -1,
  ERR_IO = -2,
  ERR_PARAM = 3,
  ERR_UNKNOWN = 4,
} error_t;

char *read_file(char *path, error_t *error);
char *clone_input(char **input, error_t *error);

#endif
