//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
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
    if (a == 0 && b == 0) return -1;
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
            int n;
            printf("Enter non-negative integer (0-20): ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            long long result = factorial(n);
            if (result == -1) {
                printf("Invalid input or overflow.\n");
            } else {
                printf("Factorial(%d) = %lld\n", n, result);
            }
            break;
        }
        case 2: {
            int n;
            printf("Enter non-negative integer (0-46): ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            int result = fibonacci(n);
            if (result == -1) {
                printf("Invalid input or overflow.\n");
            } else {
                printf("Fibonacci(%d) = %d\n", n, result);
            }
            break;
        }
        case 3: {
            int a, b;
            printf("Enter two integers: ");
            if (scanf("%d %d", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            int result = gcd(a, b);
            if (result == -1) {
                printf("Invalid input.\n");
            } else {
                printf("GCD(%d, %d) = %d\n", a, b, result);
            }
            break;
        }
    }
    
    return 0;
}