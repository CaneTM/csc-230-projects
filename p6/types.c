/**
   @file types.c
   @author Canaan Matias (ctmatias)
   Implementation of linked list nodes for int values, real values
   (doubles) and string values.
*/

#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Function used as the print method for an integer node.
 * 
 * @param n the node to print the data of
*/
static void printIntNode( Node const *n )
{
  // printf( "%d", * (int *) ( n->data ) );
  printf("%s\n", n->data);
}

/**
 * Function used as the comparison method for an integer nodes. 
 * 
 * @param a the first node to compare
 * @param b the second node to compare
 * @return true if a and b are the same node, false otherwise
*/
static bool equalsIntNode( Node const *a, Node const *b )
{
  // return b->print == printIntNode &&
  //   * ( (int *) a->data ) == * ( (int *) b->data );
  return !strcmp(a->data, b->data);
}

Node *makeIntNode( char const *init )
{
  // Allocate space for the node plus a little more for its value.
  Node *n = (Node *) malloc( sizeof( Node ) + sizeof( int ) );

  sscanf(init, "%s", n->data);

  // Validate each char in data
  for (int i = 0; i < n->data[i]; i++) {
    char ch = n->data[i];

    // One of the chars is a non-digit (invalid)
    if (!isdigit(ch)) {
      // Number is negative, so ignore
      if (ch == '-') {
        continue;
      }
      free(n);
      return NULL;
    }
  }

  // Fill in pointers for its methods defined above.
  n->print = printIntNode;
  n->equals = equalsIntNode;
  return n;
}

/** 
 * Function used as the print method for a real node. 
 * 
 * @param n the node to print the data of
*/
static void printRealNode( Node const *n )
{
  char *eptr;
  double val = strtod(n->data, &eptr);
  printf("%.3f\n", val);
}

/** 
 * Function used as the comparison method for a real node.
 * 
 * @param a the first node to compare
 * @param b the second node to compare
 * @return true if a and b are the same node, false otherwise
*/
static bool equalsRealNode( Node const *a, Node const *b )
{
  // return b->print == printRealNode && !strcmp(a->data, b->data);
  return !strcmp(a->data, b->data);
}

Node *makeRealNode( char const *init )
{
  // Allocate space for the node and its double value.
  Node *n = (Node *) malloc(sizeof(Node) + sizeof(double));

  sscanf(init, "%s", n->data);

  // Validate each char in data
  int i = 0;
  bool has_decimal = false;

  for (; i < n->data[i]; i++) {
    char ch = n->data[i];

    // Found a non-digit char (invalid)
    if (!isdigit(ch)) {

      // If decimal point, make sure there's only one
      if (ch == '.') {
        if (!has_decimal) {
          has_decimal = true;
        } else {
          free(n);
          return NULL;
        }
      } else {
        // Number is negative, so ignore
        if (ch == '-') {
          continue;
        }
        free(n);
        return NULL;
      }
    }
  }

  if (!has_decimal) {
    free(n);
    return NULL;
  }

  // Fill in pointers for its methods defined above.
  n->print = printRealNode;
  n->equals = equalsRealNode;

  return n;
}

/** 
 * Function used as the print method for a string node. 
 * 
 * @param n the node to print the data of
*/
static void printStringNode( Node const *n )
{
  // Exclude end quotes when printing
  for (int i = 1; n->data[i]; i++) {
    printf("%c", n->data[i]);
  }

  printf("\n");
}

/** 
 * Function used as the comparison method for a string node.
 * 
 * @param a the first node to compare
 * @param b the second node to compare
 * @return true if a and b are the same node, false otherwise
*/
static bool equalsStringNode( Node const *a, Node const *b )
{
  // return b->print == printStringNode && !strcmp(a->data, b->data);
  char str1[MAX_LEN] = "\0";
  char str2[MAX_LEN] = "\0";

  int offset = 1;

  // Make a copy of strings a and b
  for (int i = offset; a->data[i]; i++) {
    str1[i - offset] = a->data[i];
  }
  for (int j = offset; b->data[j]; j++) {
    str2[j - offset] = b->data[j];
  }

  int str2len = strlen(str2);
  return !strncmp(str1, str2, str2len);
}

Node *makeStringNode( char const *init )
{
  int data_len = strlen(init);

  // Allocate space for the node its string value.
  Node *n = (Node *) malloc(sizeof(Node) + data_len + 1);

  sscanf(init, "%[^\n] ", n->data);

  if (!data_len || !strstr(n->data, "\"")) {
    free(n);
    return NULL;
  }

  // Removes any space after the second end quote
  int num_quotes = 0;
  int i = 0;
  while (n->data[i]) {

    if (num_quotes == NUM_QUOTES) {
      n->data[i] = '\0';
      break;
    }

    if (n->data[i] == '\"') {
      num_quotes++;
    }
  }

  // Initialize remaining fields
  n->print = printStringNode;
  n->equals = equalsStringNode;

  return n;
}
