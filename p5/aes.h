/**
 * @file aes.h
 * @author Canaan Matias (ctmatias)
 *
 * Provides an interface for preprocessor macros
 * and function prototypes for the aes component
 */

#ifndef _AES_H_
#define _AES_H_

#include "field.h"

/** Number of bytes in an AES key or an AES block. */
#define BLOCK_SIZE 16

/** Numer of rows when a data block is arranged in a square. */
#define BLOCK_ROWS 4

/** Numer of columns when a data block is arranged in a square. */
#define BLOCK_COLS 4

/** Number of bytes in a word. */
#define WORD_SIZE 4

/** Number of roudns for 128-bit AES. */
#define ROUNDS 10

/** Index position of the first word */
#define WORD1 0

/** Index position of the second word */
#define WORD2 1

/** Index position of the third word */
#define WORD3 2

/** Index position of the fourth word */
#define WORD4 3

/** Max index position of the first word */
#define WORD1_SECT 4

/** Max index position of the second word */
#define WORD2_SECT 8

/** Max index position of the third word */
#define WORD3_SECT 12

/** Max index position of the fourth word */
#define WORD4_SECT 16

/** Max index value of the 2D square array */
#define MAX_SQUARE_IDX 3

/**
 * Computes the g function used in generating the subkeys from the original, 16-byte key
 * 
 * @param dest the 4-byte result of the g function
 * @param src the 4-byte input of the g function
 * @param r a round number between 1 and 10 (inclusive)
*/
void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r );

/**
 * Fills in the subkey array with subkeys for each round of AES
 * 
 * @param subkey the subkey array to fill
 * @param key the key to use to fill the sukey array with
*/
void generateSubkeys( byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );

/**
 * Performs the addSubkey operation, adding the given subkey (key) to the given data array.
 * 
 * @param data the data array to add to
 * @param key the subkey to add to the data array with
*/
void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );

/**
 * Rearranges a block of 16 data values from the one-dimensional arrangement to the 4 × 4 arrangement.
 * 
 * @param square the 4 by 4 array to fill
 * @param data the array to rearrange
*/
void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ] );

/**
 * Rearranges a 4 × 4 arrangement of data values, returning them as a one-dimensional array of 16 values
 * 
 * @param data the one-dimensional array to fill
 * @param square the 4 by 4 array to rearrange
*/
void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * Performs the shiftRows operation on the given 4 × 4 square of values.
 * 
 * @param square the two-dimensional array to shift rows on
*/
void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * Performs the inverse shiftRows operation on the given 4 × 4 square of values.
 * 
 * @param square the two-dimensional array to unshift rows on
*/
void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * Performs the mixColumns operation on the given 4 × 4 square of values, multiplying each column by the matrix
 * 
 * @param square the two-dimensional array to mix the columns of
*/
void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * Performs the inverse of the mixColumns operation on the given 4 × 4 square of values, 
 * multiplying each column by the inverse of the mixColumns matrix
 * 
 * @param square the two-dimensional array to unmix the columns of
*/
void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * Encrypts a 16-byte block of data using the given key. It generates the 11 subkeys from key, 
 * adds the first subkey, then performs the 10 rounds of operations needed to encrypt the block.
 * 
 * @param data the data to encrypt
 * @param key the key to encrypt with
*/
void encryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );

/**
 * Decrypts a 16-byte block of data using the given key. It generates the 11 subkeys from key, 
 * then performs the 10 rounds of inverse operations (and then an addSubkey) to decrypt the block.
 * 
 * @param data the data to decrypt
 * @param key the key to decrypt with
*/
void decryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );

#endif
