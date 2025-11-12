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

int fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    int fib_n_minus_1 = fibonacci(n - 1);
    int fib_n_minus_2 = fibonacci(n - 2);
    if (fib_n_minus_1 == -1 || fib_n_minus_2 == -1) return -1;
    if (fib_n_minus_1 > INT_MAX - fib_n_minus_2) return -1;
    return fib_n_minus_1 + fib_n_minus_2;
}

int main() {
    int choice, n;
    
    printf("Numeric Computation Program\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (n < 0) {
        printf("Input must be non-negative\n");
        return 1;
    }
    
    if (choice == 1) {
        long result = factorial(n);
        if (result == -1) {
            printf("Overflow or invalid input\n");
            return 1;
        }
        printf("Factorial of %d is %ld\n", n, result);
    } else {
        if (n > 46) {
            printf("Input too large for Fibonacci (max 46)\n");
            return 1;
        }
        int result = fibonacci(n);
        if (result == -1) {
            printf("Overflow or invalid input\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %d\n", n, result);
    }
    
    return 0;
}