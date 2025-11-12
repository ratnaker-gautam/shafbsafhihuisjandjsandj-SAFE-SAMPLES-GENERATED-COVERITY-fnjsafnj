//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
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
    long fib1 = fibonacci(n - 1);
    long fib2 = fibonacci(n - 2);
    if (fib1 == -1 || fib2 == -1) return -1;
    long result = fib1 + fib2;
    if (result < 0) return -1;
    return result;
}

int main() {
    int choice, n;
    printf("Choose computation:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("Enter n (0-20): ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (n < 0 || n > 20) {
        printf("n must be between 0 and 20.\n");
        return 1;
    }
    
    long result;
    if (choice == 1) {
        result = factorial(n);
        if (result == -1) {
            printf("Overflow in factorial computation.\n");
            return 1;
        }
        printf("Factorial(%d) = %ld\n", n, result);
    } else {
        result = fibonacci(n);
        if (result == -1) {
            printf("Overflow in fibonacci computation.\n");
            return 1;
        }
        printf("Fibonacci(%d) = %ld\n", n, result);
    }
    
    return 0;
}