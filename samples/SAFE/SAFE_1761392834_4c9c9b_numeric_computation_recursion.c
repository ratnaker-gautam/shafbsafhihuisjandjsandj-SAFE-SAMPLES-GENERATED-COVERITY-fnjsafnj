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

int read_positive_int(const char* prompt, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < 0) {
            printf("Please enter a non-negative number.\n");
            continue;
        }
        if (value > max_val) {
            printf("Value too large. Maximum allowed is %d.\n", max_val);
            continue;
        }
        return value;
    }
}

int main(void) {
    printf("Recursive Numeric Computation Demo\n");
    printf("==================================\n\n");
    
    int choice;
    while (1) {
        printf("Choose operation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        
        choice = read_positive_int("Enter choice (1-3): ", 3);
        if (choice == -1) {
            printf("Input error.\n");
            return 1;
        }
        
        if (choice == 3) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice == 1) {
            int n = read_positive_int("Enter number for factorial (0-20): ", 20);
            if (n == -1) {
                printf("Input error.\n");
                return 1;
            }
            long long result = factorial(n);
            if (result == -1) {
                printf("Error: Overflow occurred.\n");
            } else {
                printf("%d! = %lld\n", n, result);
            }
        } else if (choice == 2) {
            int n = read_positive_int("Enter Fibonacci position (0-50): ", 50);
            if (n == -1) {
                printf("Input error.\n");
                return 1;
            }
            long long result = fibonacci(n);
            if (result == -1) {
                printf("Error: Overflow occurred.\n");
            } else {
                printf("Fibonacci(%d) = %lld\n", n, result);
            }
        } else {
            printf("Invalid choice. Please try again.\n");
        }
        printf("\n");
    }
    
    return 0;
}