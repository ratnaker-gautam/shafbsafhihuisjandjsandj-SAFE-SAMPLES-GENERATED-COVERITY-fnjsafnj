//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long recursive_factorial(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    if (n > 20) {
        return -1;
    }
    long result = n * recursive_factorial(n - 1);
    if (result < 0) {
        return -1;
    }
    return result;
}

int main(void) {
    char input[32];
    int number;
    
    printf("Enter a non-negative integer (0-20) to compute factorial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (temp < 0 || temp > INT_MAX) {
        fprintf(stderr, "Invalid input: out of range\n");
        return 1;
    }
    
    number = (int)temp;
    
    long result = recursive_factorial(number);
    if (result == -1) {
        fprintf(stderr, "Error: factorial computation failed\n");
        return 1;
    }
    
    printf("Factorial of %d is %ld\n", number, result);
    return 0;
}