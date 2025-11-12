//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
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
    long fib1 = fibonacci(n - 1);
    long fib2 = fibonacci(n - 2);
    if (fib1 < 0 || fib2 < 0) return -1;
    long result = fib1 + fib2;
    if (result < 0) return -1;
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
            printf("Enter a non-negative integer: ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Input must be non-negative.\n");
                return 1;
            }
            long fact_result = factorial(num1);
            if (fact_result < 0) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Factorial of %d is %ld\n", num1, fact_result);
            break;
            
        case 2:
            printf("Enter a non-negative integer: ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Input must be non-negative.\n");
                return 1;
            }
            long fib_result = fibonacci(num1);
            if (fib_result < 0) {
                printf("Overflow occurred.\n");
                return 1;
            }
            printf("Fibonacci number at position %d is %ld\n", num1, fib_result);
            break;
            
        case 3:
            printf("Enter two integers: ");
            if (scanf("%d %d", &num1, &num2) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 == 0 && num2 == 0) {
                printf("GCD is undefined for (0,0).\n");
                return 1;
            }
            int gcd_result = gcd(num1, num2);
            printf("GCD of %d and %d is %d\n", num1, num2, gcd_result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}