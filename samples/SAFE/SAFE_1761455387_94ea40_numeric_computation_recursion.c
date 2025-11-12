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
    int fib1 = fibonacci(n - 1);
    int fib2 = fibonacci(n - 2);
    if (fib1 < 0 || fib2 < 0) return -1;
    if (fib1 > INT_MAX - fib2) return -1;
    return fib1 + fib2;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    int choice;
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. GCD\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            int num;
            printf("Enter number (0-20): ");
            if (scanf("%d", &num) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num < 0 || num > 20) {
                printf("Number out of range.\n");
                return 1;
            }
            long long result = factorial(num);
            if (result == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Factorial of %d is %lld\n", num, result);
            break;
        }
        case 2: {
            int num;
            printf("Enter Fibonacci position (0-46): ");
            if (scanf("%d", &num) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num < 0 || num > 46) {
                printf("Position out of range.\n");
                return 1;
            }
            int result = fibonacci(num);
            if (result == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Fibonacci at position %d is %d\n", num, result);
            break;
        }
        case 3: {
            int a, b;
            printf("Enter two numbers: ");
            if (scanf("%d %d", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (a == 0 && b == 0) {
                printf("GCD undefined for (0,0).\n");
                return 1;
            }
            int result = gcd(a, b);
            printf("GCD of %d and %d is %d\n", a, b, result);
            break;
        }
    }
    
    return 0;
}