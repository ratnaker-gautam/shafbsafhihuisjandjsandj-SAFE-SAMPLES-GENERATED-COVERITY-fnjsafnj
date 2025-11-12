//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

int main() {
    int number;
    char input[32];
    
    printf("Enter a non-negative integer (0-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (val < 0 || val > INT_MAX) {
        fprintf(stderr, "Invalid input: out of range\n");
        return 1;
    }
    
    number = (int)val;
    
    long result = factorial(number);
    if (result == -1) {
        fprintf(stderr, "Error: factorial computation failed\n");
        return 1;
    }
    
    printf("Factorial of %d is %ld\n", number, result);
    return 0;
}