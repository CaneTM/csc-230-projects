/**
 * @file number_10.c
 * @author Canaan Matias (ctmatias)
 * 
 * Reads numeric values from input and writes results to output. 
 * This specific implementation handles numbers in base 10.
*/

#include "number.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int skip_space()
{
    int current_char = getchar();

    // Ignore whitespace until a non-whitespace character is read
    while (current_char == ' ') {
        current_char = getchar();
    }
    
    return current_char;
}

long parse_value()
{
    // Value being parsed so far
    long value = 0;
    // Get next input character
    int current_char = skip_space();

    bool is_negative = false;
    bool has_loop_run = false;

    // Determine if number is negative
    if (current_char == '-') {
        is_negative = true;
        current_char = skip_space();
    }

    // Continue reading until reaching non base 10 digit
    while (ASCII_CODE_0 <= current_char && current_char <= ASCII_CODE_9) {
        has_loop_run = true;

        // Convert to base 10 digit
        long digit = current_char - ASCII_CODE_0;
        value *= BASE_10;
        value += digit;
        current_char = skip_space();
    }
    
    // Put current_char back on the input stream
    ungetc(current_char, stdin);

    if (!has_loop_run) {
        exit(FAIL_INPUT);
    }

    return is_negative ? -value : value;
}

void print_value(long val)
{
    printf("%ld", val);
}
