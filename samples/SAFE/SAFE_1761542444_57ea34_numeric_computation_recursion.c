//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    long long result = a + b;
    if (result < a) return -1;
    return result;
}

int get_valid_int(const char* prompt, int min, int max) {
    char buffer[32];
    char* endptr;
    long value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        if (value > INT_MAX || value < INT_MIN) {
            printf("Value out of range.\n");
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    printf("Numeric Computation with Recursion\n");
    printf("==================================\n");
    
    int choice;
    do {
        printf("\nChoose operation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        
        choice = get_valid_int("Enter choice (1-3): ", 1, 3);
        
        if (choice == 1) {
            int n = get_valid_int("Enter number for factorial (0-20): ", 0, 20);
            long long result = factorial(n);
            if (result == -1) {
                printf("Error: Overflow occurred.\n");
            } else {
                printf("Factorial of %d is %lld\n", n, result);
            }
        } else if (choice == 2) {
            int n = get_valid_int("Enter Fibonacci position (0-50): ", 0, 50);
            long long result = fibonacci(n);
            if (result == -1) {
                printf("Error: Overflow occurred.\n");
            } else {
                printf("Fibonacci at position %d is %lld\n", n, result);
            }
        }
    } while (choice != 3);
    
    printf("Goodbye!\n");
    return 0;
}