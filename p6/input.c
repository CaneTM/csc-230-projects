/**
 * @file input.c
 * @author Canaan Matias (ctmatias)
 *
 * Helps with reading user commands to manipulate the queue
 */

#include <stdio.h>
#include <stdlib.h>
#include "input.h"

char *readLine(FILE *fp)
{
  // Allocate memory to store the string being read
  char *line = (char *) malloc(sizeof(char) * (LINE_LEN + 1));

  // Uses regex expression to read until hitting a newline
  int matches = fscanf(fp, "%[^\n] ", line);

  // Only return if a non-empty line was read
  if (matches > 0) {
    return line;
  }
  else {
    free(line);
    return NULL;
  }
}
