/**
 * @file operation.c
 * @author Canaan Matias (ctmatias)
 *
 * Provides functions for performing the five arithmetic operations on signed long values. 
 * Automatically detects overflow or divide-by-zero.
 */

#include "operation.h"
#include <stdlib.h>
#include <limits.h>

long plus(long a, long b) 
{
    long sum = a + b;

    // Overflow when a positive plus a positive make a negative
    if (a > 0 && b > 0 && sum < 0) {
        exit(OUTSIDE_LONG_RANGE);
    }

    // Overflow when a negative plus a negative make a positive
    if (a < 0 && b < 0 && sum > 0) {
        exit(OUTSIDE_LONG_RANGE);
    }

    return sum;
}

long minus(long a, long b)
{
    long difference = a - b;

    // Overflow when a negative minus a positive make a positive
    if (a < 0 && b > 0 && difference > 0) {
        exit(OUTSIDE_LONG_RANGE);
    }

    // Overflow when a positive minus a negative make a negative
    if (a > 0 && b < 0 && difference < 0) {
        exit(OUTSIDE_LONG_RANGE);
    }

    return difference;
}

long divide(long a, long b)
{
    if (b == 0) {
        exit(DIVIDE_BY_ZERO_ERR);
    }

    // Overflow when a is LONG_MIN and b is -1
    if (a == LONG_MIN && b == -1) {
        exit(OUTSIDE_LONG_RANGE);
    }

    return a / b;
}

long times(long a, long b)
{
    long same_sign_threshold = divide(LONG_MAX, b);
    long diff_sign_threshold = divide(LONG_MIN, b);    

    // Case 1: a and b are both positive
    if (a > 0 && b > 0 && a > same_sign_threshold) {
        exit(OUTSIDE_LONG_RANGE);
    }

    // Case 2: a and b are both negative
    else if (a < 0 && b < 0 && a < same_sign_threshold) {
        exit(OUTSIDE_LONG_RANGE);
    }

    // Case 3: a is posiive, b is negative
    else if (a > 0 && b < 0 && a > diff_sign_threshold) {
        exit(OUTSIDE_LONG_RANGE);
    }
    
    // Case 4: a is negative, b is positive
    else if (a < 0 && b > 0 && a < diff_sign_threshold) {
        exit(OUTSIDE_LONG_RANGE);
    }
    
    return a * b;
}

long exponential(long a, long b)
{
    if (b < 0) {
        exit(NEGATIVE_EXPONENT);
    }

    long result = 1;

    for (int i = 0; i < b; i++) {
        result = times(result, a);
    }
    
    return result;
}
