/**
 * @file field.h
 * @author Canaan Matias (ctmatias)
 * 
 * Provides an interface for preprocessor macros
 * and function prototypes for the field component
 */

#ifndef _FIELD_H_
#define _FIELD_H_

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/** Number of bits in a byte. */
#define BBITS 8

/** Represents 0x11B in decimal */
#define REDUCER 283

/** Number of bits in the REDUCER */
#define REDUCER_BIT_COUNT 9

/**
 * Performs the addition operation in the 8-bit Galois field used by AES
 * 
 * @param a the first byte to add
 * @param b the second byte to add
 * @return the sum of a and b
*/
byte fieldAdd( byte a, byte b );

/**
 * Performs the subtraction operation in the 8-bit Galois field used by AES.
 * 
 * @param a the minuend
 * @param b the subtrahend
 * @return the result of subtracting b from a
*/
byte fieldSub( byte a, byte b );

/**
 * Performs the multiplication operation in the 8-bit Galois field used by AES
 * 
 * @param a the first byte to multiply
 * @param b the second byte to multiply
 * @return the product of a and b
*/
byte fieldMul( byte a, byte b );

#endif
