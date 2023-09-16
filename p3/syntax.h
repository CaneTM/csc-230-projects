/**
 * @file syntax.h
 * @author Canaan Matias (ctmatias)
 *
 * Provides an interface for syntax.c.
 * Provides function prototypes and macrodefinitions
 * for the syntax component.
 */

#include <stdbool.h>

/** Maximum length of a string that holds only 1 character */
#define CHAR_LEN 2

/**
 * Returns true if the given string is a legal identifier
 *
 * @param ident an identifier to check the validity of
 * @return true if the given string is a legal identifier, false otherwise
 */
bool validIdentifier(char ident[]);

/**
 * Finds occurrences of the target identifier in a line of text from the input file.
 * Sets the corresponding elements of the color array to the ident color.
 *
 * @param word the identifier to search for
 * @param line the line of text to look for the identifier in
 * @param color a color array used to keep track of the color of each character in line
 * @return true if the target identifier has been found in the given line, false otherwise
 */
bool markIdentifier(char word[], char line[], int color[]);
