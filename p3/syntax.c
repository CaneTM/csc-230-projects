/**
 * @file syntax.c
 * @author Canaan Matias (ctmatias)
 *
 * Takes care of finding identifiers in a line of an input file.
 */

#include "syntax.h"
#include <stdlib.h>
#include <string.h>

#include "io.h"

/**
 * Returns true if the given character is an 
 * uppercase or lowercase letter, false otherwise
 * 
 * @param ch the character to check 
 * @return true if the given character is an uppercase or lowercase letter, false otherwise
*/
static bool isLetter(char ch) {
  return ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z');
}

/**
 * Returns true if the given character is an underscore, false otherwise
 * 
 * @param ch the character to check 
 * @return true if the given character is an underscore, false otherwise
*/
static bool isUnderscore(char ch) {
  return ch == '_';
}

/**
 * Returns true if the given character is a digit from 0 to 9, false otherwise
 * 
 * @param ch the character to check 
 * @return true if the given character is a digit from 0 to 9, false otherwise
*/
static bool isDigit(char ch) {
  return '0' <= ch && ch <= '9';
}

bool validIdentifier(char ident[])
{
  // Determine if first char is valid
  if (!isLetter(ident[0]) && !isUnderscore(ident[0])) {
    return false;
  } 

  // Determine if remaining characters are valid
  for (int i = 1; ident[i]; i++) {

    if (!isLetter(ident[i]) && !isDigit(ident[i]) && !isUnderscore(ident[i])) {
      return false;
    }
  }

  return true;
}

bool markIdentifier(char word[], char line[], int color[])
{
  // Check preconditions
  if (!validIdentifier(word) || strlen(line) > LINE_LIMIT) {
    exit(EXIT_FAILURE);
  }

  bool found_identifier = false;

  // Keep track of current index of color
  int color_idx = 0;

  int limit = strlen(line) - strlen(word);

  for (int i = 0; i <= limit; i++) {
    // An array to store a substring of length strlen(word)
    char substr[strlen(word) + 1];

    color_idx = i;

    // Get a substring of length strlen(word) from line
    memcpy(substr, &line[i], strlen(word));
    substr[strlen(word)] = '\0';

    char before_ch[CHAR_LEN];
    char after_ch[CHAR_LEN];

    // Get the char before substr
    if (i != 0) {
      before_ch[0] = line[i - 1];
      before_ch[1] = '\0';
    }
    // Get the char after substr
    if (i != (strlen(line) - strlen(word))) {
      after_ch[0] = line[i + strlen(word)];
      after_ch[1] = '\0';
    }
    // Check if word and substr are not the same, 
    // the char before substr is an identifier,
    // or if the char after substr is an identifier
    if (strcmp(substr, word) || (i != 0 && validIdentifier(before_ch)) || (i != (strlen(line) - strlen(word)) && validIdentifier(after_ch))) {

      for (int j = 0; j < strlen(word); j++) {
        color[color_idx++] = DEFAULT_COLOR;
      }

      continue;
    }

    // If above condition does not run, that means we found an identifier
    for (int j = 0; j < strlen(word); j++) {
      color[color_idx++] = IDENT_COLOR;
    }
    // When done, make the char immediately after the identifier the DEFAULT_COLOR
    color[color_idx] = DEFAULT_COLOR;

    // Skip the next strlen(word) chars
    i += (strlen(word) - 1);

    found_identifier = true;
  }

  return found_identifier;
}
