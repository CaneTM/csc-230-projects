/**
 * @file number.h
 * @author Canaan Matias (ctmatias)
 * 
 * Provides an interface for number_10.c and number_12.c.
 * Provides function prototypes for reading arithmetic input,
 * writing results as output, and constants for exit status codes.
*/

/** Exit status indicating that the program received a value that's outside the range of a signed long */
#define OVERFLOW_DETECTED 100

/** Exit status indicating that the program was given invalid input. */
#define FAIL_INPUT 102

/** ASCII code for the character 0 */
#define ASCII_CODE_0 48

/** ASCII code for the character 9 */
#define ASCII_CODE_9 57

/** ASCII code for the character E */
#define ASCII_CODE_E 69

/** ASCII code for the character X */
#define ASCII_CODE_X 88

/** Decimal representation of E (from base 12) */
#define DECIMAL_E 11

/** Decimal representation of X (from base 12) */
#define DECIMAL_X 10

/** Base value for converting to base-10 */
#define BASE_10 10

/** Base value for converting to base-12 */
#define BASE_12 12

/**
 * Reads characters from standard input. Keeps reading characters
 * until it reaches a non-whitespace character or EOF. Returns the
 * code for the first non-whitespace character it finds (or EOF).
 * 
 * @return the character code for the first non-whitespace character it finds
*/
int skip_space();

/**
 * Reads the next number from the input. Terminates 
 * the program upon parsing an invalid input.
 * 
 * @return the number it parsed
*/
long parse_value();

/**
 * Prints the given value to standard output
 * 
 * @param val the value to print
*/
void print_value(long val);