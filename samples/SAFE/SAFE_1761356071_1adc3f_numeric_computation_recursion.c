//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    if (n > 20) {
        return -1;
    }
    long result = n * factorial(n - 1);
    if (result < 0) {
        return -1;
    }
    return result;
}

int main(void) {
    int input;
    char buffer[32];
    
    printf("Enter an integer (0-20) to compute factorial: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer) {
        fprintf(stderr, "Invalid input: not a number\n");
        return 1;
    }
    if (val < INT_MIN || val > INT_MAX) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    input = (int)val;
    
    if (input < 0) {
        fprintf(stderr, "Input must be non-negative\n");
        return 1;
    }
    
    long result = factorial(input);
    if (result == -1) {
        fprintf(stderr, "Error: input out of valid range (0-20) or overflow\n");
        return 1;
    }
    
    printf("Factorial of %d is %ld\n", input, result);
    return 0;
}