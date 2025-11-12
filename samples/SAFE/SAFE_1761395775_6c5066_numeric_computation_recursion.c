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
    if (a < 0 || b < 0) return -1;
    int result = a + b;
    if (result < a) return -1;
    return result;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    int choice, num1, num2;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. Greatest Common Divisor\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter non-negative integer: ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Input must be non-negative.\n");
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
            printf("Enter non-negative integer: ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Input must be non-negative.\n");
                return 1;
            }
            if (num1 > 40) {
                printf("Input too large for Fibonacci.\n");
                return 1;
            }
            int fib = fibonacci(num1);
            if (fib == -1) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Fibonacci number %d is %d\n", num1, fib);
            break;
            
        case 3:
            printf("Enter two integers: ");
            if (scanf("%d %d", &num1, &num2) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 == 0 && num2 == 0) {
                printf("Both numbers cannot be zero.\n");
                return 1;
            }
            int result = gcd(num1, num2);
            printf("GCD of %d and %d is %d\n", num1, num2, result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}