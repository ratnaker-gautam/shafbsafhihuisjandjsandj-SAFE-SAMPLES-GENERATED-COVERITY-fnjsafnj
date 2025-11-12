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
        
        return value;
    }
}

int main() {
    printf("Recursive Numeric Computation Demo\n");
    printf("==================================\n\n");
    
    int choice;
    while (1) {
        printf("Choose computation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        
        choice = read_positive_int("Enter choice (1-3): ");
        if (choice == -1) {
            printf("Input error. Exiting.\n");
            return 1;
        }
        
        if (choice == 3) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            continue;
        }
        
        if (choice == 1) {
            int n = read_positive_int("Enter number for factorial: ");
            if (n == -1) {
                printf("Input error.\n");
                continue;
            }
            
            if (n > 20) {
                printf("Number too large for factorial computation.\n");
                continue;
            }
            
            long result = factorial(n);
            if (result == -1) {
                printf("Overflow occurred in factorial computation.\n");
            } else {
                printf("Factorial of %d is %ld\n", n, result);
            }
        }
        else if (choice == 2) {
            int n = read_positive_int("Enter position for Fibonacci: ");
            if (n == -1) {
                printf("Input error.\n");
                continue;
            }
            
            if (n > 46) {
                printf("Number too large for Fibonacci computation.\n");
                continue;
            }
            
            long result = fibonacci(n);
            if (result == -1) {
                printf("Overflow occurred in Fibonacci computation.\n");
            } else {
                printf("Fibonacci at position %d is %ld\n", n, result);
            }
        }
        
        printf("\n");
    }
    
    return 0;
}