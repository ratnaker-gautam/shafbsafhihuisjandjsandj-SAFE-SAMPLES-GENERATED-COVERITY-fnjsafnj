//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
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

int read_positive_int(const char* prompt, int max_val) {
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
            printf("Please enter a non-negative number.\n");
            continue;
        }
        
        if (value > max_val) {
            printf("Number too large. Maximum allowed is %d.\n", max_val);
            continue;
        }
        
        break;
    }
    
    return value;
}

int main() {
    printf("Factorial Calculator\n");
    printf("====================\n");
    
    int max_input = 20;
    int n = read_positive_int("Enter a number (0-20): ", max_input);
    
    if (n < 0) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    long result = factorial(n);
    
    if (result == -1) {
        printf("Error: Factorial calculation failed (overflow or invalid input).\n");
        return 1;
    }
    
    printf("Factorial of %d is %ld\n", n, result);
    
    return 0;
}