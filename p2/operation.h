/**
 * @file operation.h
 * @author Canaan Matias (ctmatias)
 * 
 * Provides an interface for operation.c.
 * Provides function prototypes for arithmetic operations
 * and constants for exit status codes.
*/

/** Exit status indicating that the program received a value that's outside the range of a signed long */
#define OUTSIDE_LONG_RANGE 100

/** Exit status indicating that there was an attempt to divide by zero */
#define DIVIDE_BY_ZERO_ERR 101

/** Exit status indicating that the user entered a negative exponent */
#define NEGATIVE_EXPONENT 103

/**
 * Adds the given parameters and returns the sum.
 * Automatically detects overflow.
 * 
 * @param a the first number to add
 * @param b the second number to add
 * @return the sum of a and b
*/
long plus(long a, long b);

/**
 * Subtracts b from a and returns the difference.
 * Automatically detects overflow.
 * 
 * @param a the minuend
 * @param b the subtrahend
 * @return the difference of a and b
*/
long minus(long a, long b);

/**
 * Multiplies a and b and returns the product.
 * Automatically detects overflow.
 * 
 * @param a the first number to multiply
 * @param b the second number to multiply
 * @return the product of a and b
*/
long times(long a, long b);

/**
 * Exponentiates the parameters (i.e. raises a to the power of b).
 * Automatically detects overflow and negative exponents.
 * 
 * @param a the base
 * @param b the exponent
 * @return the value of a to the bth power
*/
long exponential(long a, long b);

/**
 * Divides a by b and returns the quotient.
 * Automatically detects overflow and any attempt to divide by zero.
 * 
 * @param a the dividend
 * @param b the divisor
 * @return the quotient of a and b
*/
long divide(long a, long b);
