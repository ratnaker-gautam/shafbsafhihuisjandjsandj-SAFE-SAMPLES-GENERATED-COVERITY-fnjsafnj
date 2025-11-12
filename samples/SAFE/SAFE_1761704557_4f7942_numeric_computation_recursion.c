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
    printf("%s", prompt);
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    char* endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (value < 0 || value > INT_MAX) return -1;
    return (int)value;
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("========================\n");
    
    int choice;
    do {
        printf("\nChoose operation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        printf("Enter choice (1-3): ");
        
        char buffer[32];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        
        char* endptr;
        long ch = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input\n");
            continue;
        }
        choice = (int)ch;
        
        switch (choice) {
            case 1: {
                int n = read_positive_int("Enter non-negative integer for factorial: ");
                if (n < 0) {
                    printf("Invalid input\n");
                    break;
                }
                long result = factorial(n);
                if (result < 0) {
                    printf("Overflow or invalid input\n");
                } else {
                    printf("%d! = %ld\n", n, result);
                }
                break;
            }
            case 2: {
                int n = read_positive_int("Enter non-negative integer for Fibonacci: ");
                if (n < 0) {
                    printf("Invalid input\n");
                    break;
                }
                long result = fibonacci(n);
                if (result < 0) {
                    printf("Overflow or invalid input\n");
                } else {
                    printf("Fibonacci(%d) = %ld\n", n, result);
                }
                break;
            }
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (choice != 3);
    
    return 0;
}