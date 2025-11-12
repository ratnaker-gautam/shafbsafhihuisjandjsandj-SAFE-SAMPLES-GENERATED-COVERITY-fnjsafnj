//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 92) return -1;
    long a = fibonacci(n - 1);
    long b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    if (a > LONG_MAX - b) return -1;
    return a + b;
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
            printf("Enter number (0-20): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            long fact = factorial(num1);
            if (fact == -1) {
                printf("Invalid input or overflow.\n");
            } else {
                printf("Factorial of %d is %ld\n", num1, fact);
            }
            break;
            
        case 2:
            printf("Enter position (0-92): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            long fib = fibonacci(num1);
            if (fib == -1) {
                printf("Invalid input or overflow.\n");
            } else {
                printf("Fibonacci at position %d is %ld\n", num1, fib);
            }
            break;
            
        case 3:
            printf("Enter two numbers: ");
            if (scanf("%d %d", &num1, &num2) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (num1 == 0 && num2 == 0) {
                printf("GCD undefined for (0,0).\n");
            } else {
                int result = gcd(num1, num2);
                printf("GCD of %d and %d is %d\n", num1, num2, result);
            }
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}