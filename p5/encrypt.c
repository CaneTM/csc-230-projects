/**
 * @file encrypt.c
 * @author Canaan Matias (ctmatias)
 *
 * Main component of the encrypt program.
 * Reads the input file to perform AES encryption
 * and writes to the ciphertext output.
 */

#include <stdlib.h>
#include <stdio.h>

#include "io.h"
#include "aes.h"

/** Valid number of arguments */
#define NUM_ARGS 4

/**
 * Parses the given command-line arguments
 * 
 * @param argc number of arguments
 * @param argv array of char pointers for each argument
*/
static void processArgs(int argc, char const *argv[])
{
  // Check for valid num of args
  if (argc != NUM_ARGS) {
    fprintf(stderr, "usage: encrypt <key-file> <input-file> <output-file>\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * Checks the sizes of the given key and data inputs.
 * Terminates the program if keysize isn't exactly 16 bytes.
 * Terminates the program if datasize isn't a multiple of 16 bytes.
 * 
 * @param keysize the size of the key (in bytes)
 * @param datasize the size of the input data (in bytes)
*/
static void checkSizes(int keysize, int *datasize, byte *data, char const *argv[])
{
  // Check the key size
  if (keysize != BLOCK_SIZE) {
    fprintf(stderr, "Bad key file: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // Check the data size
  if ((*datasize) % BLOCK_SIZE != 0) {
    // Add padding at the end
    int remainder = (*datasize) % BLOCK_SIZE;
    int padding = BLOCK_SIZE - remainder;
    // Calculate the new size
    size_t new_size = (*datasize) + padding;
    // Reallocate memory
    data = realloc(data, new_size);

    // Pad the data
    for (int i = (*datasize); i < new_size; i++) {
      *(data + i) = 0x00;
    }

    // Update data size
    *datasize = new_size;

    // fprintf(stderr, "Bad plaintext file length: %s", argv[2]);
    // exit(EXIT_FAILURE);
  }
}

/**
 * Entry point of program
 *
 * @param argc number of command-line args
 * @param argv array of command-line args
 * @return exit status code
 */
int main(int argc, char const *argv[])
{
  processArgs(argc, argv);

  int keysize = 0;
  int datasize = 0;

  // Read the key and the data
  byte *key = readBinaryFile(argv[1], &keysize);
  byte *data = readBinaryFile(argv[2], &datasize);

  // Check the sizes of the key and input data
  checkSizes(keysize, &datasize, data, argv);

  // Encrypt the data
  byte data_copy[BLOCK_SIZE];
  int byte16_sections = datasize / BLOCK_SIZE;

  for (int i = 0; i < byte16_sections; i++) {
    
    // Copy 16 bytes of data at a time
    for (int j = 0; j < BLOCK_SIZE; j++) {
      data_copy[j] = *(data + (BLOCK_SIZE * i + j));
    }

    encryptBlock(data_copy, key);

    // Re-copy the encrypted 16 bytes
    for (int j = 0; j < BLOCK_SIZE; j++) {
      *(data + (BLOCK_SIZE * i + j)) = data_copy[j];
    }
  }

  // Write the encryption to the given output file
  writeBinaryFile(argv[3], data, datasize);

  free(key);
  free(data);

  return EXIT_SUCCESS;
}
