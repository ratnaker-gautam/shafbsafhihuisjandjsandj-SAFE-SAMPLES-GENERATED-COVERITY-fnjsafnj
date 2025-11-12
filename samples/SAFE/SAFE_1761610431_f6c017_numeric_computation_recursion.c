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
    long long result = n * factorial(n - 1);
    if (result < 0) {
        return -1;
    }
    return result;
}

int read_positive_int(const char* prompt) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < 0) {
            printf("Please enter a non-negative integer.\n");
            continue;
        }
        
        if (value > 20) {
            printf("Value too large. Maximum allowed is 20.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

int main(void) {
    int n = read_positive_int("Enter a non-negative integer (0-20): ");
    
    if (n < 0) {
        return EXIT_FAILURE;
    }
    
    long long result = factorial(n);
    
    if (result == -1) {
        printf("Error: Integer overflow occurred.\n");
        return EXIT_FAILURE;
    }
    
    printf("Factorial of %d is %lld\n", n, result);
    
    return EXIT_SUCCESS;
}