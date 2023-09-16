/**
 * @file io.h
 * @author Canaan Matias (ctmatias)
 * 
 * Provides an interface for io.c.
 * Provides function prototypes and macrodefinitions
 * for the io component.
*/

#include <stdio.h>
#include <stdbool.h>

/** Maximum length of an input line. */
#define LINE_LIMIT 100

/** Used for element of the output line in the default color. */
#define DEFAULT_COLOR 0

/** Used for element of the output line colored like that target identifier. */
#define IDENT_COLOR 1

/** For the extra credit, this is the color value for operators  */
#define OP_COLOR 2

/**
 * Counts and returns the number of lines in the provided file.
 * Terminates the program with an error if any line in the file is too long.
 * 
 * @param fp a pointer to the input file stream
 * @return the number of lines in the file
*/
int countLines(FILE *fp);

/**
 * Reads the next line from the given file and stores it in the given string
 * 
 * @param fp a pointer to the input file stream
 * @param line a string variable to store the line of text read
 * @return true if there's a line to be read, false otherwise
*/
bool readLine(FILE *fp, char line[LINE_LIMIT + 1]);

/**
 * Prints the given string to standard output. The color of a particular character
 * in the given string is determined by its corresponding value in the given int array.
 * 
 * @param line the string to be printed
 * @param color an integer array with each int corresponding to a character in line
*/
void printLine(char line[LINE_LIMIT + 1], int color[LINE_LIMIT]);
