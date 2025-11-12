//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
        printf("Choose computation:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Exit\n");
        
        choice = read_positive_int("Enter choice (1-3): ", 3);
        if (choice == -1) continue;
        
        if (choice == 3) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice == 1) {
            int n = read_positive_int("Enter number for factorial (0-20): ", 20);
            if (n == -1) continue;
            long result = factorial(n);
            if (result == -1) {
                printf("Error: Computation overflow or invalid input.\n");
            } else {
                printf("%d! = %ld\n", n, result);
            }
        } else if (choice == 2) {
            int n = read_positive_int("Enter Fibonacci position (0-40): ", 40);
            if (n == -1) continue;
            long result = fibonacci(n);
            if (result == -1) {
                printf("Error: Computation overflow or invalid input.\n");
            } else {
                printf("Fibonacci(%d) = %ld\n", n, result);
            }
        }
        
        printf("\n");
    }
    
    return 0;
}