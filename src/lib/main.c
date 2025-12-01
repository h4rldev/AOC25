#include <lib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline static char *throw_error(error_t *error_buf, error_t error,
                                char *message, ...) {
  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);

  *error_buf = error;
  return NULL;
}

char *read_file(char *path, error_t *error) {
  size_t size = 0;
  char *buffer = NULL;
  FILE *file = NULL;

  if (!path)
    return throw_error(error, ERR_PARAM, "Missing path pointer");

  file = fopen(path, "rb");
  if (!file)
    return throw_error(error, ERR_IO, "Could not open file, quitting");

  if (fseek(file, 0L, SEEK_END)) {
    fclose(file);
    return throw_error(error, ERR_IO, "Could not seek to end of file");
  }

  size = ftell(file);
  if (fseek(file, 0L, SEEK_SET)) {
    fclose(file);
    return throw_error(error, ERR_IO, "Could not seek to beginning of file");
  }

  buffer = (char *)malloc(size);
  if (!buffer) {
    fclose(file);
    return throw_error(error, ERR_MEM, "Could not allocate memory for buffer");
  }

  size_t amount_read = fread(buffer, 1, size, file);
  fclose(file);

  if (amount_read != size) {
    free(buffer);
    return throw_error(error, ERR_IO,
                       "Amount read is more or less than file %lf",
                       amount_read);
  }

  return buffer;
}
