/**
 * @file ident.c
 * @author Canaan Matias (ctmatias)
 * 
 * Responsible for parsing the command-line arguments and using 
 * the other components to read the input file and report matching lines.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "io.h"
#include "syntax.h"

/** Number of arguments required on the command line. */
#define REQUIRED_ARGS 2

/** Lines of context to show around an identifier. */
static int context = 0;

/** True if we're printing line numbers. */
static bool numbers = false;
  
/** True if we're showing operators (for the extra credit). */
// static bool operators = false;

/**
 * Parses the optional command-line arguments -n and -c
 * 
 * @param argc number of arguments
 * @param argv array of char pointers for each argument
*/
static void processArgs(int argc, char const *argv[])
{
  // Incorrect number of args
  if ((argc - 1) < REQUIRED_ARGS) {
    fprintf(stderr, "usage: ident [-c <context>] [-n] <file> <identifier>\n");
    exit(EXIT_FAILURE);
  }

  // Get the identifier
  char ident[strlen(argv[argc - 1]) + 1];
  strcpy(ident, argv[argc - 1]);

  // Invalid identifier detected
  if (!validIdentifier(ident)) {
    fprintf(stderr, "Invalid identifier: %s\n", ident);
    exit(EXIT_FAILURE);
  }

  // Visit all the arguments except the first
  for (int i = argc - 1; i > 0; i--) {
    // Check for invalid options
    if (i < (argc - 2) && strcmp("-n", argv[i]) && strcmp("-c", argv[i]) && !atoi(argv[i])) {
      fprintf(stderr, "usage: ident [-c <context>] [-n] <file> <identifier>\n");
      exit(EXIT_FAILURE);
    }

    // User has opted to show line numbers
    if (!strcmp("-n", argv[i])) {
      numbers = true;
    }
    // If a digit is encontered, user has potentially 
    // opted for context reporting
    else if (atoi(argv[i])) {
      // Valid context reporting syntax detected
      if (!strcmp("-c", argv[i - 1])) {
        context = atoi(argv[i]);
      }
      // Invalid -c syntax detected
      else {
        fprintf(stderr, "usage: ident [-c <context>] [-n] <file> <identifier>\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

/**
 * Prints the line number at the start of each output line
 * 
 * @param num_lines total number of lines in the input file
 * @param line_num the line number to print
*/
static void printLineNum(int num_lines, int line_num)
{
  // Count the number of digits in num_lines
  int digit_count = 0;
  int n = num_lines;

  while (n != 0) {
    n /= 10;
    ++digit_count;
  }

  printf("%*d: ", digit_count, line_num);
}

/**
 * Sets all of the elements of the given 2D history array to null
 * 
 * @param history a 2D array of strings used to store previously read lines
*/
static void resetHistory(char history[][LINE_LIMIT + 1])
{
  for (int i = 0; i < context; i++) {
    for (int j = 0; j < (LINE_LIMIT + 1); j++) {
      history[i][j] = '\0';
    }
  }
}

/**
 * Stores previously read lines of an input file
 * 
 * @param line the most recently read line to store
 * @param history a 2D array of strings used to store previously read lines
*/
static void storeHistory(char line[], char history[][LINE_LIMIT + 1])
{
  // Start at row i=1
  int i;

  // Copy string from row i to row i-1
  for (i = 1; i < context; i++) {
    strcpy(history[i - 1], history[i]);
  }

  // Copy line into the last row
  strcpy(history[i - 1], line);
}

/**
 * Prints the number of lines of "before-context" specified by the user
 * 
 * @param history a 2D array of strings used to store previously read lines
 * @param num_lines total number of lines in the user-given input file
 * @param line_num a pointer to the variable containing the current line number
*/
static void printBeforeContext(char history[][LINE_LIMIT + 1], int num_lines, int line_num)
{
  for (int i = 0; i < context; i++) {
    if (history[i][0] == '\0') {
      continue;
    }

    int before_context_line_num = line_num - context + i;

    if (numbers) {
      printLineNum(num_lines, before_context_line_num);
    }

    printf("%s", history[i]);
  }

  // Reset history
  resetHistory(history);
}

/**
 * Prints the line containing the identifier
 * 
 * @param num_lines total number of lines in the user-given input file
 * @param line_num the current line number
 * @param line the line to be printed
 * @param color an int array parallel to line that contains the color of each character
*/
static void printMatchingLine(int num_lines, int line_num, char line[LINE_LIMIT + 1], int color[LINE_LIMIT + 1])
{
  // Print line number of matching line
  if (numbers) {
    printLineNum(num_lines, line_num);
  }

  // Print matching line
  printLine(line, color);
}

/**
 * Prints the number of lines of "after-context" specified by the user
 * 
 * @param input_stream a pointer to the input file stream
 * @param num_lines total number of lines in the user-given input file
 * @param line_num a pointer to the variable containing the current line number
 * @param line the line to be printed
 * @param color an int array parallel to line that contains the color of each character
 * @param ident the string to be searched for
 * @param missed_lines_ptr a pointer to the variable containing the number of lines that wouldn't have been printed otherwise 
*/
static void printAfterContext(FILE *input_stream, int num_lines, int *line_num, char line[LINE_LIMIT + 1], int color[LINE_LIMIT + 1], char ident[], int *missed_lines_ptr)
{
  for (int i = 1; i <= context; i++) {
    // Stop printing if you've reached the end of the file
    if (*line_num >= num_lines) {
      break;
    }

    readLine(input_stream, line);

    int after_context_line_num = *line_num + 1;

    if (numbers) {
      printLineNum(num_lines, after_context_line_num);
    }

    // If an after-context line contains
    // an identifier, account for it
    if (markIdentifier(ident, line, color)) {
      *missed_lines_ptr = *missed_lines_ptr + 1;
      printLine(line, color);
    }
    else {
      printf("%s", line);
    }

    // Increment line_num after calling readLine()
    *line_num = *line_num + 1;
  }
}

/**
 * Entry point of program
 * 
 * @param argc number of provided arguments
 * @param argv an array of strings representing each argument
 * @return exit status code
*/
int main(int argc, char const *argv[])
{
  processArgs(argc, argv);

  // Get the identifier
  char ident[strlen(argv[argc - 1]) + 1];
  strcpy(ident, argv[argc - 1]);
  ident[strlen(ident)] = '\0';

  // Attempt to open file
  FILE *input_stream = fopen(argv[argc - 2], "r");

  // Invalid file detected
  if (!input_stream) {
    fprintf(stderr, "Can't open file: %s\n", argv[argc - 2]);
    exit(EXIT_FAILURE);
  }

  // Get the number of lines in the input file
  int num_lines = countLines(input_stream);
  // Keep track of the current line number
  int line_num = 0;

  // Store the history for context reporting,
  // as well as the current row in history
  char history[context][LINE_LIMIT + 1];
  
  // Initialize all elements in history to null
  resetHistory(history);

  // If a matching line appears as an "after-context"
  // line, increment this variable by one so we can 
  // account for it when we print the "after-context"
  // of said matching line
  int missed_lines_after_context = 0;

  char line[LINE_LIMIT + 1];
  int color[LINE_LIMIT + 1];
  
  // Start reading each line in the input file
  while (readLine(input_stream, line)) {
    line_num++;

    // Print the lines of "after-context" that
    // the program would not have otherwise printed
    // if a matching line is part of another matching
    // line's "after-context"
    if (missed_lines_after_context) {
      if (numbers) {
        printLineNum(num_lines, line_num);
      }
      printf("%s", line);
      missed_lines_after_context--;
      continue;
    }

    // Store history if user wants context 
    if (context && !markIdentifier(ident, line, color)) {
      storeHistory(line, history);
    }

    if (markIdentifier(ident, line, color)) {

      // Print n lines before the matching line
      if (context) {
        printBeforeContext(history, num_lines, line_num);
      }

      printMatchingLine(num_lines, line_num, line, color);

      // Print n lines after the matching line
      if (context && line_num < num_lines) {
        printAfterContext(input_stream, num_lines, &line_num, line, color, ident, &missed_lines_after_context);
      }
    }
  }

  fclose(input_stream);

  return EXIT_SUCCESS;
}
