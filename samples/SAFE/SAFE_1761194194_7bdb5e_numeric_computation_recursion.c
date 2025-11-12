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
    
    long a = fibonacci(n - 1);
    long b = fibonacci(n - 2);
    
    if (a == -1 || b == -1) return -1;
    if (a > LONG_MAX - b) return -1;
    
    return a + b;
}

double power(double base, int exponent) {
    if (exponent < 0) {
        if (base == 0.0) return -1.0;
        return 1.0 / power(base, -exponent);
    }
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    double half = power(base, exponent / 2);
    if (half == -1.0) return -1.0;
    
    if (exponent % 2 == 0) {
        return half * half;
    } else {
        return half * half * base;
    }
}

int main() {
    int choice, n;
    double base;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. Power\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter non-negative integer: ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (n < 0) {
                printf("Input must be non-negative\n");
                return 1;
            }
            long fact = factorial(n);
            if (fact == -1) {
                printf("Overflow or invalid input\n");
                return 1;
            }
            printf("Factorial(%d) = %ld\n", n, fact);
            break;
            
        case 2:
            printf("Enter non-negative integer: ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (n < 0) {
                printf("Input must be non-negative\n");
                return 1;
            }
            long fib = fibonacci(n);
            if (fib == -1) {
                printf("Overflow or invalid input\n");
                return 1;
            }
            printf("Fibonacci(%d) = %ld\n", n, fib);
            break;
            
        case 3:
            printf("Enter base: ");
            if (scanf("%lf", &base) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            printf("Enter exponent: ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            double result = power(base, n);
            if (result == -1.0) {
                printf("Invalid operation (division by zero)\n");
                return 1;
            }
            printf("%.2f^%d = %.6f\n", base, n, result);
            break;
    }
    
    return 0;
}