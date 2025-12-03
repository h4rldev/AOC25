#include <lib.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_combo_p1(char *line) {
  int digits[1024];
  int digit_count = strlen(line);

  if (strlen(line) < 2)
    return -1;

  for (int i = 0; i < strlen(line); i++) {
    digits[i] = line[i] - '0';
  }

  int max_value = -1, best_first = -1, best_second = -1;

  for (int i = 0; i < digit_count; i++) {
    int first = digits[i];
    int max_after = -1;

    for (int j = i + 1; j < digit_count; j++) {
      if (digits[j] > max_after)
        max_after = digits[j];
    }

    if (max_after != -1) {
      int two_digit_combo = first * 10 + max_after;
      if (two_digit_combo > max_value) {
        max_value = two_digit_combo;
        best_first = first;
        best_second = max_after;
      }
    }
  }

  return max_value;
}

uint64_t combine_arr(int *arr, int arr_len) {
  long long result = 0;
  for (int i = 0; i < arr_len; i++) {
    int num = arr[i];
    int digits = 0;
    int temp = num == 0 ? 1 : num; // Handle 0
    while (temp) {
      digits++;
      temp /= 10;
    }
    result = result * pow(10, digits) + num;
  }
  return result;
}

uint64_t find_combo_p2(char *line) {
  int digits[1024];

  const unsigned int digit_count = strlen(line);
  const unsigned int result_buf_len = 12;
  const unsigned int digits_to_remove = digit_count - result_buf_len;

  int result_buf[12];

  if (digit_count < result_buf_len)
    return -1;

  for (int i = 0; i < strlen(line); i++) {
    digits[i] = line[i] - '0';
  }

  int stack[1024];
  int stack_size = 0;

  for (int i = 0; i < digit_count; i++) {
    int current = digits[i];
    printf("current: %d\n", current);

    while (stack_size > 0 && digits_to_remove > 0 &&
           current > stack[stack_size - 1] &&
           stack_size + (digit_count - i - 1) >= result_buf_len)
      stack_size--;

    if (stack_size < result_buf_len)
      stack[stack_size++] = current;
  }

  for (int i = 0; i < result_buf_len; i++) {
    result_buf[i] = stack[i];
  }

  return combine_arr(result_buf, result_buf_len);
}

uint64_t part1(char **input) {
  char *token = NULL;
  int64_t sum = 0;

  int buffer_copy_len = strlen(*input);
  char *buffer_copy = malloc(buffer_copy_len + 1);
  strcpy(buffer_copy, *input);

  token = strtok(buffer_copy, "\n");
  while (token != NULL) {
    int combined = find_combo_p1(token);
    printf("combined: %d\n", combined);
    sum += combined;

    printf("sum: %ld\n", sum);

    token = strtok(NULL, "\n");
  }

  free(buffer_copy);
  return sum;
}

uint64_t part2(char **input) {
  char *token = NULL;
  uint64_t sum = 0;

  int buffer_copy_len = strlen(*input);
  char *buffer_copy = malloc(buffer_copy_len + 1);
  strcpy(buffer_copy, *input);

  token = strtok(buffer_copy, "\n");
  while (token != NULL) {
    uint64_t combined = find_combo_p2(token);
    printf("combined: %ld\n", combined);
    sum += combined;

    printf("sum: %ld\n", sum);

    token = strtok(NULL, "\n");
  }

  free(buffer_copy);
  return sum;
}

int main(void) {
  error_t error;
  char *input = read_file("inputs/03.txt", &error);
  if (error != ERR_OK)
    return 1;

  printf("Meow1: %ld\n", part1(&input));
  printf("Meow2: %ld\n", part2(&input));

  free(input);
  return 0;
}
