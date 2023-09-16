/**
 * @file input.h
 * @author Canaan Matias (ctmatias)
 *
 * Provides function prototypes, macrodefinitions, and types for the input component
 */

#include <stdio.h>

/** Maximum length of an input line */
#define LINE_LEN 1000

/**
 * Reads a single line of input from the given input stream and
 * returns it as a string inside a block of dynamically allocated memory
 *
 * @param fp the input stream to read from
 * @return a pointer to a string in dynamically allocated memory
 */
char *readLine(FILE *fp);
