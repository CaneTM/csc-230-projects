/**
 * @file field.c
 * @author Canaan Matias (ctmatias)
 *
 * Implements support for basic arithmetic operations in an 8-bit (1 byte) Galois field.
 * Used internally by AES for many steps in the encryption and decryption process
 */

#include "field.h"
#include <stdlib.h>
#include <stdio.h>

byte fieldAdd( byte a, byte b )
{
  return a ^ b;
}

byte fieldSub( byte a, byte b )
{
  return a ^ b;
}

/**
 * Computes a 16-bit result as the first phase of fieldMul
 * 
 * @param a the first byte to multiply
 * @param b the second byte to multiply
 * @return a 16-bit result of multiplying a and b
*/
static unsigned short fieldMulPhase1( byte a, byte b )
{
  unsigned short result = 0;

  for (int i = 0; i < BBITS * sizeof(byte); i++) {
    // If the bit is a 1
    if (b & (1 << i)) {
      // Left shift by i bits and XOR-accumulate the result
      result ^= (a << i);
    }
  }

  return result;
}

/**
 * Counts the number of bits in the given value
 * 
 * @param value the value to count the bits of
 * @return the number of bits in value
*/
static int getBitCount( unsigned short value )
{
  int bit_count = 0;
  unsigned short value_copy = value;

  while (value_copy) {
    bit_count += 1;
    value_copy >>= 1;
  }

  return bit_count;
}

/**
 * Compresses the given 16-bit value to 8 bits
 * 
 * @param value the 16-bit value to compress
 * @return an 8-bit version of the given value
*/
static byte fieldMulPhase2( unsigned short value )
{
  // Count how many bits value has
  int bit_count = getBitCount(value);

  // Find out how much you initially need to left-shift
  int diff = bit_count - REDUCER_BIT_COUNT;

  unsigned short value_copy = value;

  while (diff >= 0) {

    // If the leftmost bit in value is a 1
    if ((value_copy >> (bit_count - 1))) {
      // Left-shift the reducer by diff amount
      // and XOR it with value
      value_copy ^= (REDUCER << diff);
    }

    bit_count--;
    diff--;
  }

  return value_copy;
}

byte fieldMul( byte a, byte b )
{
  // Phase 1: compute a 16-bit result
  unsigned short result = fieldMulPhase1(a, b);
  // Phase 2: compress it to 8-bits
  return fieldMulPhase2(result);
}
