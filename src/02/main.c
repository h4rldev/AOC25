#include <lib.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int64_t part1(char *input) {
  char *token = NULL;
  char i_string[256] = {0};

  regex_t regex;
  int reti;

  int64_t start = 0;
  int64_t end = 0;
  int64_t invalid_buf = 0;

  const char *pattern = "^([0-9]+)\\1$";
  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    return -1;
  }

  token = strtok(input, ",");
  while (token != NULL) {
    // printf("token: %s\n", token);
    sscanf(token, "%ld-%ld", &start, &end);

    for (int64_t i = start; i <= end; i++) {
      sprintf(i_string, "%ld", i);

      if (strlen(i_string) % 2 != 0)
        continue;

      reti = regexec(&regex, i_string, 0, NULL, 0);
      if (reti == 0) {
        // printf("i: %s\n", i_string);
        invalid_buf += i;
      }
    }

    token = strtok(NULL, ",");
  }

  regfree(&regex);
  return invalid_buf;
}

int64_t part2(char *input) {
  char *token = NULL;
  char i_string[256] = {0};

  regex_t regex;
  int reti;

  int64_t start = 0;
  int64_t end = 0;
  int64_t invalid_buf = 0;

  const char *pattern = "^([0-9]+)\\1+$";
  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    return -1;
  }

  token = strtok(input, ",");
  while (token != NULL) {
    // printf("token: %s\n", token);
    sscanf(token, "%ld-%ld", &start, &end);

    for (int64_t i = start; i <= end; i++) {
      sprintf(i_string, "%ld", i);

      reti = regexec(&regex, i_string, 0, NULL, 0);
      if (reti == 0) {
        // printf("i: %s\n", i_string);
        invalid_buf += i;
      }
    }

    token = strtok(NULL, ",");
  }

  regfree(&regex);
  return invalid_buf;
}

int main(void) {
  error_t error;
  char *input1 = read_file("inputs/02.txt", &error);
  if (error != ERR_OK)
    return 1;

  char *input2 = read_file("inputs/02.txt", &error);
  if (error != ERR_OK)
    return 1;

  printf("Meow1: %ld\n", part1(input1));
  printf("Meow2: %ld\n", part2(input2));

  free(input1);
  free(input2);
  return 0;
}
