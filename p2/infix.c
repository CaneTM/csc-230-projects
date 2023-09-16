/**
 * @file infix.c
 * @author Canaan Matias (ctmatias)
 * 
 * The top-level component in the program.
 * Reads and evaluates an arithmetic expression 
 * (either in base-10 or base-12) and outputs the result. 
*/

#include "number.h"
#include "operation.h"
#include <stdio.h>
#include <stdlib.h>

static long parse_exp();

static long parse_mul_div();

/**
 * Entry point of program. Reads and evaluates
 * the lowest-precedence parts of an expression.
 * 
 * @return program exit status
*/
int main()
{
    // Store result of expression here
    long result = parse_mul_div();
    // Get the next character
    int next_char = skip_space();

    while (next_char == '+' || next_char == '-') {
        // Perform addition 
        if (next_char == '+') {
            result = plus(result, parse_mul_div());
        }
        // Perform subtraction 
        else if (next_char == '-') {
            result = minus(result, parse_mul_div());
        }

        next_char = skip_space();
    }

    // Terminate if the next character is an invalid character
    if (next_char != ' ' && next_char != '\n') {
        exit(FAIL_INPUT);
    }
    
    print_value(result);
    printf("\n");

    return EXIT_SUCCESS;
}

/**
 * Reads the highest-precedence parts of an expression
 * (e.g. an individual number or an exponentiation)
 * 
 * @return the value that the expression evaluates to
*/
static long parse_exp()
{
    // Store the first value it reads
    long current_val = parse_value();
    // Get the next character
    int next_char = skip_space();

    while (next_char == '^') {
        current_val = exponential(current_val, parse_value());
        next_char = skip_space();
    }
    
    // Put char back onto the stream
    ungetc(next_char, stdin);

    return current_val;
}

/**
 * Reads the second-highest precedence parts of an expression (i.e. a sequence  
 * of one or more factors with multiply and/or divide operators in between them)
 * 
 * @return the value that the input term evaluates to
*/
static long parse_mul_div()
{
    // Store the first value it reads
    long current_val = parse_exp();
    // Get the next character
    int next_char = skip_space();

    while (next_char == '*' || next_char == '/') {
        // Perform multiplication 
        if (next_char == '*') {
            current_val = times(current_val, parse_exp());
        }
        // Perform division 
        else if (next_char == '/') {
            current_val = divide(current_val, parse_exp());
        }

        next_char = skip_space();
    }

    // Put char back onto the stream
    ungetc(next_char, stdin);

    return current_val;
}
