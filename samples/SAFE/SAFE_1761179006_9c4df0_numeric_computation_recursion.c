//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
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
    long temp = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (temp < 0 || temp > 20) {
        fprintf(stderr, "Input out of range: must be between 0 and 20\n");
        return 1;
    }
    
    number = (int)temp;
    
    long result = factorial(number);
    if (result == -1) {
        fprintf(stderr, "Error: integer overflow in calculation\n");
        return 1;
    }
    
    printf("Factorial of %d is %ld\n", number, result);
    return 0;
}