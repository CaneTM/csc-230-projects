/**
 * @file io.h
 * @author Canaan Matias (ctmatias)
 * 
 * Provides an interface for preprocessor macros
 * and function prototypes for the io component
*/

#include "field.h"

/** Max number of bytes to read */
#define MAX_BYTES 4096

/** Number of bytes of each section of data */
#define DATA_SIZE 16

/**
 * Reads the contents of the binary file with the given name
 * 
 * @param filename the file to read from
 * @param size an integer pointer whose value is filled with the total size of the given file
 * @return a pointer to a dynamically allocated array of bytes containing the entire file contents
*/
byte *readBinaryFile( char const *filename, int *size );

/**
 * Writes the contents of the given data array (in binary) to the file with the given name
 * 
 * @param filename the file to write to
 * @param data the data to write
 * @param size number of bytes contained in the data array
*/
void writeBinaryFile( char const *filename, byte *data, int size );
