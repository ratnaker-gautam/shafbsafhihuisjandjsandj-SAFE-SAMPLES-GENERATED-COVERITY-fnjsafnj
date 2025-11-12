//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    if (n > 20) {
        return -1;
    }
    long long result = n * factorial(n - 1);
    if (result < 0) {
        return -1;
    }
    return result;
}

int main(void) {
    char input[32];
    int num;
    
    printf("Enter an integer (0-20) to compute factorial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not an integer\n");
        return 1;
    }
    if (val < 0 || val > 20) {
        fprintf(stderr, "Input out of range (0-20)\n");
        return 1;
    }
    if (val > INT_MAX) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    num = (int)val;
    long long result = factorial(num);
    
    if (result == -1) {
        fprintf(stderr, "Error computing factorial\n");
        return 1;
    }
    
    printf("Factorial of %d is %lld\n", num, result);
    return 0;
}