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

int main() {
    int choice, n;
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (n < 0) {
        printf("Input must be non-negative.\n");
        return 1;
    }
    
    if (choice == 1) {
        if (n > 20) {
            printf("Input too large for factorial computation.\n");
            return 1;
        }
        long long result = factorial(n);
        if (result == -1) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", n, result);
    } else {
        if (n > 50) {
            printf("Input too large for Fibonacci computation.\n");
            return 1;
        }
        long long result = fibonacci(n);
        if (result == -1) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %lld\n", n, result);
    }
    
    return 0;
}