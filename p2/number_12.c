/**
 * @file number_12.c
 * @author Canaan Matias (ctmatias)
 * 
 * Reads numeric values from input and writes results to output. 
 * This specific implementation handles numbers in base 12.
*/

#include "number.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Multiplies a and b and returns the product.
 * Automatically detects overflow.
 * Used for the parse_value() function.
 * 
 * @param a the first number to multiply
 * @param b the second number to multiply
 * @return the product of a and b
*/
static long multiply(long a, long b)
{
    long same_sign_threshold = LONG_MAX / b;
    long diff_sign_threshold = LONG_MIN / b;    

    // Case 1: a and b are both positive
    if (a > 0 && b > 0 && a > same_sign_threshold) {
        exit(OVERFLOW_DETECTED);
    }

    // Case 2: a and b are both negative
    else if (a < 0 && b < 0 && a < same_sign_threshold) {
        exit(OVERFLOW_DETECTED);
    }

    // Case 3: a is posiive, b is negative
    else if (a > 0 && b < 0 && a > diff_sign_threshold) {
        exit(OVERFLOW_DETECTED);
    }
    
    // Case 4: a is negative, b is positive
    else if (a < 0 && b > 0 && a < diff_sign_threshold) {
        exit(OVERFLOW_DETECTED);
    }
    
    return a * b;
}

/**
 * Adds the given parameters and returns the sum.
 * Automatically detects overflow.
 * Used for the parse_value() function.
 * 
 * @param a the first number to add
 * @param b the second number to add
 * @return the sum of a and b
*/
static long add(long a, long b) 
{
    long sum = a + b;

    // Overflow when a positive plus a positive make a negative
    if (a > 0 && b > 0 && sum < 0) {
        exit(OVERFLOW_DETECTED);
    }

    // Overflow when a negative plus a negative make a positive
    if (a < 0 && b < 0 && sum > 0) {
        exit(OVERFLOW_DETECTED);
    }

    return sum;
}

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

    // Continue reading until reaching a non base 12 digit
    while ((ASCII_CODE_0 <= current_char && current_char <= ASCII_CODE_9) || current_char == ASCII_CODE_X || current_char == ASCII_CODE_E) {
        has_loop_run = true;

        // Convert from base 12 to base 10
        long digit = current_char - ASCII_CODE_0;

        if (current_char == ASCII_CODE_X) {
            digit = DECIMAL_X;
        }
        else if (current_char == ASCII_CODE_E) {
            digit = DECIMAL_E;
        }

        value = multiply(value, BASE_12);
        value = add(value, digit);
        current_char = skip_space();
    }

    if (!has_loop_run) {
        exit(FAIL_INPUT);
    }

    // Put current_char back on the input stream
    ungetc(current_char, stdin);

    return is_negative ? -value : value;
}

/**
 * Prints a given digit using its base 12 character representation
 * 
 * @param digit the digit to print in base 12
*/
static void print_base_12_char(long digit)
{
    if (digit == DECIMAL_X) {
        printf("X");
    }
    else if (digit == DECIMAL_E) {
        printf("E");
    }
    else {
        printf("%ld", digit);
    }
}

void print_value(long val)
{
    long value = val;

    long quotient = value / BASE_12;    
    long remainder = value % BASE_12;   

    // Base case
    if (quotient == 0) {
        print_base_12_char(remainder);
        return;
    }

    else {
        // Make quotient and remainder positive so they're easier to parse
        if (quotient < 0) {
            // This is here to ensure that only the first quotient is negative
            // so only one negative sign can be printed and subsequent quotients
            // can be positive
            quotient = -quotient;
            printf("-");
        }
        remainder = remainder < 0 ? -remainder : remainder;
        print_value(quotient);
        print_base_12_char(remainder);
    }
}
