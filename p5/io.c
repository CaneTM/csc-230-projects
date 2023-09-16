/**
 * @file io.c
 * @author Canaan Matias (ctmatias)
 *
 * Responsible for reading input files and writing output
 * files when the encrypt or decrypt program is done.
 */

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

/**
 * Checks whether the given file is valid
 * 
 * @param fp the file stream to check
*/
static void checkFile( FILE *fp, char const *filename )
{
  if (!fp) {
    free(fp);
    fprintf(stderr, "Can't open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }
}

byte *readBinaryFile( char const *filename, int *size )
{
  // Open the input file
  FILE *src = fopen(filename, "rb");
  checkFile(src, filename);

  while (fgetc(src) != EOF) {
    *size += 1;
  }

  // Create space to store the data being read
  byte *data = malloc(*size);

  // Go back to the start of the file
  rewind(src);

  // Read the file
  fread(data, sizeof(byte), *size, src);

  // Close the file
  fclose(src);

  return data;
}

void writeBinaryFile( char const *filename, byte *data, int size )
{
  // Open the output file
  FILE *dest = fopen(filename, "wb");
  checkFile(dest, filename);

  fwrite(data, sizeof(byte), size, dest);

  // Close the output file
  fclose(dest);
}
