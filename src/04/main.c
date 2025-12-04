#include <lib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROLL '@'
#define EMPTY '.'

size_t get_height(char *input) {
  size_t height = 0;

  while (*input) {
    if (*input == '\n')
      height++;
    input++;
  }

  return height;
}

uint64_t part1(char *input) {
  char **input_2d = {NULL};
  char *token = NULL;
  char *input_clone = NULL;
  char *input_size_clone = NULL;

  error_t error = ERR_OK;

  size_t rows = 0;
  int row_idx = 0;

  size_t columns = 0;
  int column_idx = 0;

  uint64_t available_rolls = 0;

  input_clone = clone_input(&input, &error);
  if (error != ERR_OK)
    return 0;

  input_size_clone = clone_input(&input, &error);
  if (error != ERR_OK)
    return 0;

  for (char *p = input_clone; *p; p++) {
    if (*p == '\r')
      *p = '\n';
  }

  token = strtok(input_clone, "\n");
  rows = get_height(input_size_clone) + 1;
  input_2d = calloc(rows, sizeof(char *));

  size_t max_len = 0;
  columns = strlen(token);

  for (size_t i = 0; i < rows; i++) {
    input_2d[i] = calloc(columns, sizeof(char));
  }

  while (token && row_idx < rows) {
    strncpy(input_2d[row_idx], token, columns);
    row_idx++;
    token = strtok(NULL, "\n");
  }

  const int neighbor_offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                      {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  for (row_idx = 0; row_idx < rows; row_idx++) {
    for (column_idx = 0; column_idx < columns; column_idx++) {
      int roll_count = 0;

      if (input_2d[row_idx][column_idx] != ROLL)
        continue;

      for (int i = 0; i < 8; i++) {
        int selected_row = row_idx + neighbor_offsets[i][0];
        int selected_column = column_idx + neighbor_offsets[i][1];

        if (selected_row >= 0 && selected_row < (int)rows &&
            selected_column >= 0 && selected_column < (int)columns &&
            input_2d[selected_row][selected_column] == ROLL) {
          roll_count++;
        }
      }

      if (roll_count < 4) {
        available_rolls++;
      }
    }
  }

  for (size_t i = 0; i < rows; i++) {
    printf("%s\n", input_2d[i]);
    free(input_2d[i]);
  }
  free(input_2d);
  free(input_clone);
  free(input_size_clone);
  return available_rolls;
}

uint64_t part2(char *input) {
  char **input_2d = {NULL};
  char *token = NULL;
  char *input_clone = NULL;
  char *input_size_clone = NULL;

  error_t error = ERR_OK;

  size_t rows = 0;
  int row_idx = 0;

  size_t columns = 0;
  int column_idx = 0;

  uint64_t available_rolls = 0;

  input_clone = clone_input(&input, &error);
  if (error != ERR_OK)
    return 0;

  input_size_clone = clone_input(&input, &error);
  if (error != ERR_OK)
    return 0;

  for (char *p = input_clone; *p; p++) {
    if (*p == '\r')
      *p = '\n';
  }

  token = strtok(input_clone, "\n");
  rows = get_height(input_size_clone) + 1;
  input_2d = calloc(rows, sizeof(char *));

  size_t max_len = 0;
  columns = strlen(token);

  for (size_t i = 0; i < rows; i++) {
    input_2d[i] = calloc(columns, sizeof(char));
  }

  while (token && row_idx < rows) {
    strncpy(input_2d[row_idx], token, columns);
    row_idx++;
    token = strtok(NULL, "\n");
  }

  const int neighbor_offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                      {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  int changed;
  do {
    changed = 0;
    for (row_idx = 0; row_idx < rows; row_idx++) {
      for (column_idx = 0; column_idx < columns; column_idx++) {
        int roll_count = 0;

        if (input_2d[row_idx][column_idx] != ROLL)
          continue;

        for (int i = 0; i < 8; i++) {
          int selected_row = row_idx + neighbor_offsets[i][0];
          int selected_column = column_idx + neighbor_offsets[i][1];

          if (selected_row >= 0 && selected_row < (int)rows &&
              selected_column >= 0 && selected_column < (int)columns &&
              input_2d[selected_row][selected_column] == ROLL) {
            roll_count++;
          }
        }

        if (roll_count < 4) {
          input_2d[row_idx][column_idx] = 'x';
          available_rolls++;
          changed = 1;
        }
      }
    }
  } while (changed);

  for (size_t i = 0; i < rows; i++) {
    printf("%s\n", input_2d[i]);
    free(input_2d[i]);
  }
  free(input_2d);
  free(input_clone);
  free(input_size_clone);
  return available_rolls;
}

int main(void) {
  error_t error = ERR_OK;
  char *input = read_file("inputs/04.txt", &error);
  char *input_p1 = clone_input(&input, &error);
  char *input_p2 = clone_input(&input, &error);

  free(input);

  printf("Meow1: %ld\n", part1(input_p1));
  printf("Meow2: %ld\n", part2(input_p2));

  free(input_p1);
  free(input_p2);

  return 0;
}
