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
    int a = fibonacci(n - 1);
    int b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    int result = a + b;
    if (result < a) return -1;
    return result;
}

long long power(int base, int exponent) {
    if (exponent < 0) return -1;
    if (exponent == 0) return 1;
    if (base == 0) return 0;
    long long result = base * power(base, exponent - 1);
    if (result < 0) return -1;
    return result;
}

int main() {
    int choice, num1, num2;
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. Power\n");
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
        case 1:
            printf("Enter number (0-20): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 0 || num1 > 20) {
                printf("Number out of range.\n");
                return 1;
            }
            long long fact = factorial(num1);
            if (fact == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Factorial of %d is %lld\n", num1, fact);
            break;
            
        case 2:
            printf("Enter Fibonacci position (0-40): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 0 || num1 > 40) {
                printf("Position out of range.\n");
                return 1;
            }
            int fib = fibonacci(num1);
            if (fib == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Fibonacci at position %d is %d\n", num1, fib);
            break;
            
        case 3:
            printf("Enter base (1-15): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 1 || num1 > 15) {
                printf("Base out of range.\n");
                return 1;
            }
            printf("Enter exponent (0-20): ");
            if (scanf("%d", &num2) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num2 < 0 || num2 > 20) {
                printf("Exponent out of range.\n");
                return 1;
            }
            long long pow_result = power(num1, num2);
            if (pow_result == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("%d^%d = %lld\n", num1, num2, pow_result);
            break;
    }
    
    return 0;
}