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
    if (a > INT_MAX - b) return -1;
    return a + b;
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    int choice, num1, num2, n;
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. GCD\n");
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
            printf("Factorial of %d is %lld\n", n, fact);
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
            printf("Enter two integers: ");
            if (scanf("%d %d", &num1, &num2) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 == 0 && num2 == 0) {
                printf("GCD(0,0) is undefined.\n");
                return 1;
            }
            int result = gcd(num1, num2);
            printf("GCD(%d, %d) = %d\n", num1, num2, result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}