#include <lib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(char *input) {
  int start = 50;
  int amount_of_zeroes = 0;

  char *token = strtok(input, "\n");
  while (token) {
    char direction = token[0];
    int number = atoi(token + 1);

    switch (direction) {
    case 'L':
      start -= number;
      break;
    case 'R':
      start += number;
      break;

    default:
      fprintf(stderr, "Unknown direction: %c", direction);
      return -1;
    }

    if (start % 100 == 0)
      start = 0;

    if (start == 0)
      amount_of_zeroes++;

    token = strtok(NULL, "\n");
  }

  return amount_of_zeroes;
}

int part2(char *input) {
  int start = 50;
  int amount_of_zeroes = 0;

  char *token = strtok(input, "\n");
  while (token) {
    char direction = token[0];
    int number = atoi(token + 1);

    switch (direction) {
    case 'L':
      for (int i = 0; i < number; i++) {
        start -= 1;

        if (start % 100 == 0)
          start = 0;

        if (start == 0)
          amount_of_zeroes++;
      }
      break;

    case 'R':
      for (int i = 0; i < number; i++) {
        start += 1;

        if (start % 100 == 0)
          start = 0;

        if (start == 0)
          amount_of_zeroes++;
      }
      break;

    default:
      fprintf(stderr, "Unknown direction: %c", direction);
      return -1;
    }

    token = strtok(NULL, "\n");
  }

  return amount_of_zeroes;
}

int main(void) {
  error_t error;
  char *input = read_file("inputs/01.txt", &error);

  printf("Hello, World!\n");
  // printf("Meow: %d\n", part1(input));
  printf("Meow2: %d\n", part2(input));

  free(input);
  return 0;
}
