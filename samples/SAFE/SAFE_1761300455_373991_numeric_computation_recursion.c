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
            return -1;
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
            printf("Value out of integer range.\n");
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    printf("Recursive Numeric Computation\n");
    printf("==============================\n\n");
    
    int choice;
    while (1) {
        printf("Choose computation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        
        choice = get_valid_int("Enter choice (1-3): ", 1, 3);
        if (choice == -1) {
            printf("Input error.\n");
            return 1;
        }
        
        if (choice == 3) {
            printf("Goodbye!\n");
            break;
        }
        
        int n;
        long long result;
        
        switch (choice) {
            case 1:
                n = get_valid_int("Enter number for factorial (0-20): ", 0, 20);
                if (n == -1) {
                    printf("Input error.\n");
                    return 1;
                }
                result = factorial(n);
                if (result == -1) {
                    printf("Error: Computation overflow.\n");
                } else {
                    printf("%d! = %lld\n", n, result);
                }
                break;
                
            case 2:
                n = get_valid_int("Enter Fibonacci position (0-50): ", 0, 50);
                if (n == -1) {
                    printf("Input error.\n");
                    return 1;
                }
                result = fibonacci(n);
                if (result == -1) {
                    printf("Error: Computation overflow.\n");
                } else {
                    printf("Fibonacci(%d) = %lld\n", n, result);
                }
                break;
        }
        
        printf("\n");
    }
    
    return 0;
}