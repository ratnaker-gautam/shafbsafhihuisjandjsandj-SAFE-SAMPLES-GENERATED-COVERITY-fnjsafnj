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

int main(void) {
    int choice;
    printf("Choose operation:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice == 1) {
        int num;
        printf("Enter a non-negative integer (0-20): ");
        if (scanf("%d", &num) != 1) {
            printf("Invalid input.\n");
            return 1;
        }
        if (num < 0 || num > 20) {
            printf("Number out of valid range.\n");
            return 1;
        }
        long long result = factorial(num);
        if (result == -1) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", num, result);
    } else if (choice == 2) {
        int num;
        printf("Enter a non-negative integer (0-40): ");
        if (scanf("%d", &num) != 1) {
            printf("Invalid input.\n");
            return 1;
        }
        if (num < 0 || num > 40) {
            printf("Number out of valid range.\n");
            return 1;
        }
        int result = fibonacci(num);
        if (result == -1) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Fibonacci number %d is %d\n", num, result);
    } else {
        printf("Invalid choice.\n");
        return 1;
    }
    
    return 0;
}