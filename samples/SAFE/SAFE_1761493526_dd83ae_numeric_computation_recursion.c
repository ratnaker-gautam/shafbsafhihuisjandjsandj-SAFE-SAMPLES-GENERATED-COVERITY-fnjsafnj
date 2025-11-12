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

int main() {
    int choice, n;
    char input[100];
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        printf("Choice must be 1 or 2.\n");
        return 1;
    }
    
    printf("Enter a non-negative integer: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        printf("Invalid number.\n");
        return 1;
    }
    
    if (n < 0) {
        printf("Number must be non-negative.\n");
        return 1;
    }
    
    if (choice == 1) {
        long result = factorial(n);
        if (result < 0) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Factorial of %d is %ld\n", n, result);
    } else {
        if (n > 46) {
            printf("Fibonacci sequence too large.\n");
            return 1;
        }
        long result = fibonacci(n);
        if (result < 0) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %ld\n", n, result);
    }
    
    return 0;
}