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

long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    long fib1 = fibonacci(n - 1);
    long fib2 = fibonacci(n - 2);
    if (fib1 < 0 || fib2 < 0) return -1;
    long result = fib1 + fib2;
    if (result < 0) return -1;
    return result;
}

int read_positive_int(const char* prompt, int max_value) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error.\n");
            continue;
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
        
        if (value > max_value) {
            printf("Value too large. Maximum allowed is %d.\n", max_value);
            continue;
        }
        
        break;
    }
    
    return value;
}

int main() {
    printf("Numeric Computation with Recursion\n");
    printf("==================================\n\n");
    
    int choice;
    do {
        printf("Choose operation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        
        choice = read_positive_int("Enter choice (1-3): ", 3);
        
        switch (choice) {
            case 1: {
                int n = read_positive_int("Enter number for factorial (0-20): ", 20);
                long result = factorial(n);
                if (result < 0) {
                    printf("Error: Computation overflow or invalid input.\n");
                } else {
                    printf("Factorial of %d is %ld\n", n, result);
                }
                break;
            }
            case 2: {
                int n = read_positive_int("Enter position for Fibonacci sequence (0-40): ", 40);
                long result = fibonacci(n);
                if (result < 0) {
                    printf("Error: Computation overflow or invalid input.\n");
                } else {
                    printf("Fibonacci number at position %d is %ld\n", n, result);
                }
                break;
            }
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        printf("\n");
    } while (choice != 3);
    
    return 0;
}