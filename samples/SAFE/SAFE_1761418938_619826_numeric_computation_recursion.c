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
    long result = n * factorial(n - 1);
    if (result < 0 || result > LONG_MAX / n) {
        return -1;
    }
    return result;
}

int main(void) {
    int input;
    char buffer[32];
    
    printf("Enter a non-negative integer (0-20): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not an integer\n");
        return 1;
    }
    
    if (val < 0 || val > 20) {
        fprintf(stderr, "Input out of range (0-20)\n");
        return 1;
    }
    
    input = (int)val;
    
    long result = factorial(input);
    if (result == -1) {
        fprintf(stderr, "Error: overflow in computation\n");
        return 1;
    }
    
    printf("Factorial of %d is %ld\n", input, result);
    return 0;
}