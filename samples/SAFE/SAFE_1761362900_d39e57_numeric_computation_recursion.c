//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    long long result = n * factorial(n - 1);
    if (result < 0) {
        return -1;
    }
    return result;
}

long long fibonacci(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    if (a < 0 || b < 0) {
        return -1;
    }
    long long result = a + b;
    if (result < 0) {
        return -1;
    }
    return result;
}

int main() {
    int choice, n;
    printf("Choose operation:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
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
        long long result = factorial(n);
        if (result < 0) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", n, result);
    } else {
        long long result = fibonacci(n);
        if (result < 0) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %lld\n", n, result);
    }
    return 0;
}