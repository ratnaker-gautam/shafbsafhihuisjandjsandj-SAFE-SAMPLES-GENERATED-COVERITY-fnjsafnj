//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    long long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

int fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 46) return -1;
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
    if (a == 0) return b;
    return gcd(b, a % b);
}

int main() {
    int choice, num1, num2, n;
    long long fact_result;
    int fib_result, gcd_result;
    
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
            printf("Enter number (0-20): ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            fact_result = factorial(n);
            if (fact_result == -1) {
                printf("Error: Invalid input or overflow.\n");
            } else {
                printf("Factorial of %d is %lld\n", n, fact_result);
            }
            break;
            
        case 2:
            printf("Enter Fibonacci position (0-46): ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            fib_result = fibonacci(n);
            if (fib_result == -1) {
                printf("Error: Invalid input or overflow.\n");
            } else {
                printf("Fibonacci at position %d is %d\n", n, fib_result);
            }
            break;
            
        case 3:
            printf("Enter two numbers: ");
            if (scanf("%d %d", &num1, &num2) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            gcd_result = gcd(num1, num2);
            printf("GCD of %d and %d is %d\n", num1, num2, gcd_result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}