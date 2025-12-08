#include <lib.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_height(char *input) {
  int height = 0;

  while (*input) {
    if (*input == '\n')
      height++;
    input++;
  }

  return height + 1;
}

char **split_based_on_whitespace(char *input, error_t *error) {
  char **buf = {NULL};
  char *input_copy = NULL;

  size_t input_size = 0;
  size_t first_part_size = 0;
  size_t second_part_size = 0;

  input_copy = clone_input(&input, error);
  if (!input_copy)
    return throw_error(
        error, ERR_MEM, "%s: Could not allocate memory for buffer: %s\n",
        VAR_TO_STR(split_based_on_whitespace), VAR_TO_STR(input_copy));

  input_size = strlen(input_copy);

  buf = (char **)calloc(2, sizeof(char *));
  if (!buf)
    return throw_error(error, ERR_MEM,
                       "%s: Could not allocate memory for buffer: %s\n",
                       VAR_TO_STR(split_based_on_whitespace), VAR_TO_STR(buf));

  char *end = strstr(input_copy, "\n\n");
  char *start = end;
  while (*start == '\n' || *start == '\r')
    start++;
  memmove(end, start, strlen(start) + 1);

  second_part_size = strlen(end);
  first_part_size = input_size - second_part_size - 1;

  buf[0] = calloc(first_part_size, sizeof(char));
  buf[1] = calloc(second_part_size + 1, sizeof(char));

  memcpy(buf[0], input_copy, first_part_size - 1);
  memcpy(buf[1], input_copy + first_part_size, second_part_size);

  free(input_copy);
  return buf;
}

uint64_t part1(char *input) {
  char **input_parts = {NULL};
  error_t error = ERR_OK;

  uint64_t amount_of_fresh = 0;

  char *range_token = NULL;
  char *number_token = NULL;

  int range_height = 0;
  int number_height = 0;

  char **range_lines = {NULL};
  uint64_t *numbers = {0};

  uint64_t start = 0;
  uint64_t end = 0;
  uint64_t number = 0;

  input_parts = split_based_on_whitespace(input, &error);
  if (error != ERR_OK)
    return 0;

  printf("input_parts[0]: %s\n", input_parts[0]);
  printf("input_parts[1]: %s\n", input_parts[1]);

  range_height = get_height(input_parts[0]);
  number_height = get_height(input_parts[1]);

  range_lines = calloc(range_height, sizeof(char *));
  if (range_lines == NULL)
    goto cleanup;
  numbers = calloc(number_height, sizeof(uint64_t));
  if (numbers == NULL) {
    free(range_lines);
    goto cleanup;
  }

  range_token = strtok(input_parts[0], "\n");

  size_t range_idx = 0;
  while (range_token != NULL && range_idx < range_height) {
    size_t line_size = strlen(range_token) + 1;
    range_lines[range_idx] = calloc(line_size, sizeof(char));
    if (range_lines[range_idx] == NULL)
      goto cleanup;

    memcpy(range_lines[range_idx], range_token, line_size);
    range_token = strtok(NULL, "\n");
    range_idx++;
  }

  number_token = strtok(input_parts[1], "\n");
  size_t number_idx = 0;
  while (number_token != NULL) {
    uint64_t number = atol(number_token);

    numbers[number_idx++] = number;
    number_token = strtok(NULL, "\n");
  }

  for (int i = 0; i < number_height; i++) {
    // printf("numbers[%d]: %lu\n", i, numbers[i]);
    bool valid = false;
    uint64_t current_number = numbers[i];
    for (int j = 0; j < range_height; j++) {
      sscanf(range_lines[j], "%ld-%ld", &start, &end);
      if (current_number >= start && current_number <= end) {
        printf("%lu is valid because it's lower than %lu and higher than %lu\n",
               current_number, end, start);
        amount_of_fresh++;
        break;
      }
    }
  }

precleanup:
  for (int i = 0; i < range_height; i++) {
    free(range_lines[i]);
  }
  free(range_lines);
  free(numbers);

cleanup:
  free(input_parts[0]);
  free(input_parts[1]);
  free(input_parts);

  return amount_of_fresh;
}

typedef struct {
  uint64_t start;
  uint64_t end;
} range_t;

int compare_ranges(const void *mem_a, const void *mem_b) {
  range_t *range_a = (range_t *)mem_a;
  range_t *range_b = (range_t *)mem_b;

  if (range_a->start < range_b->start)
    return -1;
  if (range_a->start > range_b->start)
    return 1;

  return 0;
}

uint64_t part2(char *input) {
  char **input_parts = {NULL};
  error_t error = ERR_OK;

  uint64_t amount_of_fresh = 0;

  char *range_token = NULL;

  int range_height = 0;
  range_t *range_lines = {NULL};

  input_parts = split_based_on_whitespace(input, &error);
  // if (error != ERR_OK)
  //  return 0;

  free(input_parts[1]); // throw away the numbers since all we need are the
  // ranges for part 2

  range_height = get_height(input_parts[0]);
  range_lines = calloc(range_height, sizeof(range_t));
  if (range_lines == NULL)
    goto cleanup;

  range_token = strtok(input_parts[0], "\n");
  size_t range_idx = 0;
  while (range_token != NULL && range_idx < range_height) {
    uint64_t start = 0;
    uint64_t end = 0;

    sscanf(range_token, "%ld-%ld", &start, &end);
    range_lines[range_idx] = (range_t){start, end};

    range_token = strtok(NULL, "\n");
    range_idx++;
  }

  qsort(range_lines, range_height, sizeof(range_t), compare_ranges);

  uint64_t current_start = range_lines[0].start;
  uint64_t current_end = range_lines[0].end;
  printf("current range: %ld-%ld\n", range_lines[0].start, range_lines[0].end);

  for (int i = 1; i < range_height; i++) {
    printf("current range: %ld-%ld\n", range_lines[i].start,
           range_lines[i].end);
    if (range_lines[i].start <= current_end + 1) {
      if (range_lines[i].end > current_end)
        current_end = range_lines[i].end;
    } else {
      amount_of_fresh += (current_end - current_start + 1);
      current_start = range_lines[i].start;
      current_end = range_lines[i].end;
    }
  }

  amount_of_fresh += (current_end - current_start + 1);

precleanup:
  free(range_lines);

cleanup:
  free(input_parts[0]);
  free(input_parts);

  return amount_of_fresh;
}

int main(void) {
  error_t error = ERR_OK;

  char *input = read_file("inputs/05.txt", &error);
  if (error != ERR_OK)
    return 1;

  char *input_p1 = clone_input(&input, &error);
  if (error != ERR_OK)
    return 1;

  char *input_p2 = clone_input(&input, &error);
  if (error != ERR_OK)
    return 1;

  free(input);

  // printf("Part 1: %ld\n", part1(input_p1));
  printf("Part 2: %ld\n", part2(input_p2));

  free(input_p1);
  free(input_p2);
  return 0;
}
