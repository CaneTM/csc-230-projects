/**
 * @file io.c
 * @author Canaan Matias (ctmatias)
 * 
 * Handles reading lines of text from an input file and 
 * printing them with color highlighting as needed.
*/

#include "io.h"
#include <stdlib.h>

#include <string.h>

int countLines(FILE *fp)
{
  // Keep track of number of lines
  int num_lines = 0;
  
  // Store the current character being read
  char ch = getc(fp);
  // Keep track of character count for each line
  int ch_count = 0;

  // Determine the number of lines 
  // and whether a line is too long
  while (ch != EOF && ch_count <= LINE_LIMIT) {

    // Increment number of lines if you
    // reached the end of a line
    if (ch == '\n') {
      ch = getc(fp);
      ch_count = 0;
      num_lines++;
      continue;
    }

    ch_count++;
    ch = getc(fp);
  }

  // Terminate if a line exceeds the line limit
  if (ch_count > LINE_LIMIT) {
    fprintf(stderr, "Input line too long\n");
    exit(EXIT_FAILURE);
  }

  // Return to the start of the file
  rewind(fp);

  return num_lines;
}

bool readLine(FILE *fp, char line[LINE_LIMIT + 1])
{
  // Read in an entire line and return true if a line has been read
  return fgets(line, LINE_LIMIT + 1, fp) != NULL;
}

/**
 * Sets the print color for terminal output to red
*/
static void setRed()
{
  printf("%c%c%c%c%c", 0x1b, 0x5b, 0x33, 0x31, 0x6d);
}

/**
 * Sets the print color for terminal output to the default color
*/
static void setDefault()
{
  printf("%c%c%c%c", 0x1b, 0x5b, 0x30, 0x6d);
}

void printLine(char line[LINE_LIMIT + 1], int color[LINE_LIMIT])
{
  bool is_colored = false;

  int i;

  int line_len = strlen(line);

  // Iterate through each character and print it using its corresponding color
  for (i = 0; i < line_len; i++) {
    // Set to the default color
    if (color[i] == DEFAULT_COLOR && is_colored) {
      setDefault();
      is_colored = false;
    }
    // Set to the ident color
    else if (color[i] == IDENT_COLOR && !is_colored) {
      setRed();
      is_colored = true;
    }
    printf("%c", line[i]);
  }

  // Switch back to the default color
  // if the text was previously colored
  if (is_colored) {
    setDefault();
  }

  // Manually add a newline if a line's 
  // length is exactly LINE_LIMIT
  if (i == LINE_LIMIT) {
    printf("\n");
  }
}
