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

int main() {
    int choice, n;
    printf("Choose computation:\n1. Factorial\n2. Fibonacci\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter n (0-20): ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (n < 0 || n > 20) {
        printf("n must be between 0 and 20\n");
        return 1;
    }
    
    if (choice == 1) {
        long long result = factorial(n);
        if (result == -1) {
            printf("Overflow error\n");
            return 1;
        }
        printf("Factorial(%d) = %lld\n", n, result);
    } else {
        int result = fibonacci(n);
        if (result == -1) {
            printf("Overflow error\n");
            return 1;
        }
        printf("Fibonacci(%d) = %d\n", n, result);
    }
    
    return 0;
}