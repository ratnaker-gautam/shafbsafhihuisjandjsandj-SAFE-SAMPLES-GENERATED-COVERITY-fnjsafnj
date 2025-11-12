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
    if (fib1 == -1 || fib2 == -1) return -1;
    if (fib1 > INT_MAX - fib2) return -1;
    return fib1 + fib2;
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
    double result = half * half;
    if (exponent % 2 == 1) {
        result *= base;
    }
    return result;
}

int main() {
    int choice, n, exp;
    double base;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. Power\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter non-negative integer: ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (n < 0) {
                printf("Input must be non-negative.\n");
                return 1;
            }
            long long fact = factorial(n);
            if (fact == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Factorial(%d) = %lld\n", n, fact);
            break;
            
        case 2:
            printf("Enter non-negative integer: ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (n < 0) {
                printf("Input must be non-negative.\n");
                return 1;
            }
            int fib = fibonacci(n);
            if (fib == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Fibonacci(%d) = %d\n", n, fib);
            break;
            
        case 3:
            printf("Enter base: ");
            if (scanf("%lf", &base) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            printf("Enter exponent: ");
            if (scanf("%d", &exp) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            double pwr = power(base, exp);
            if (pwr == -1.0) {
                printf("Invalid operation.\n");
                return 1;
            }
            printf("Power(%.2f, %d) = %.6f\n", base, exp, pwr);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}