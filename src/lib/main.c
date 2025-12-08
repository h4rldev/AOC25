#include <lib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *throw_error(error_t *error_buf, error_t error, char *message, ...) {
  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);

  *error_buf = error;
  return NULL;
}

char *clone_input(char **input, error_t *error) {
  char *buf = NULL;
  size_t len = 0;

  len = strlen(*input) + 1;
  buf = (char *)malloc(len);
  if (!buf)
    return throw_error(error, ERR_MEM,
                       "%s: Could not allocate memory for buffer",
                       VAR_TO_STR(clone_input));

  strncpy(buf, *input, len);
  buf[len - 1] = '\0';

  return buf;
}

char *read_file(char *path, error_t *error) {
  size_t size = 0;
  char *buffer = NULL;
  FILE *file = NULL;

  if (!path)
    return throw_error(error, ERR_PARAM, "%s: Missing path pointer\n",
                       VAR_TO_STR(read_file));

  file = fopen(path, "rb");
  if (!file)
    return throw_error(error, ERR_IO, "%s: Could not open file: %s, quitting\n",
                       VAR_TO_STR(read_file), path);

  if (fseek(file, 0L, SEEK_END)) {
    fclose(file);
    return throw_error(error, ERR_IO, "%s: Could not seek to end of file\n",
                       VAR_TO_STR(read_file));
  }

  size = ftell(file);
  if (fseek(file, 0L, SEEK_SET)) {
    fclose(file);
    return throw_error(error, ERR_IO,
                       "%s: Could not seek to beginning of file\n",
                       VAR_TO_STR(read_file));
  }

  buffer = (char *)malloc(size);
  if (!buffer) {
    fclose(file);
    return throw_error(error, ERR_MEM,
                       "%s: Could not allocate memory for buffer\n",
                       VAR_TO_STR(read_file));
  }

  size_t amount_read = fread(buffer, 1, size, file);
  fclose(file);

  if (amount_read != size) {
    free(buffer);
    return throw_error(error, ERR_IO,
                       "%s: Amount read is more or less than file %lf",
                       VAR_TO_STR(read_file), amount_read);
  }

  buffer[size - 1] = '\0';
  return buffer;
}
