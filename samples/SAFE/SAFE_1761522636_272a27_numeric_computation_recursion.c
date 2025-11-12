//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
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
    if (fib1 < 0 || fib2 < 0) return -1;
    long result = fib1 + fib2;
    if (result < 0) return -1;
    return result;
}

int main() {
    int choice, n;
    char buffer[32];
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        fprintf(stderr, "Choice must be 1 or 2\n");
        return 1;
    }
    
    printf("Enter n (0-20): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid number\n");
        return 1;
    }
    
    if (n < 0 || n > 20) {
        fprintf(stderr, "n must be between 0 and 20\n");
        return 1;
    }
    
    long result;
    
    switch (choice) {
        case 1:
            result = factorial(n);
            if (result < 0) {
                fprintf(stderr, "Overflow in factorial\n");
                return 1;
            }
            printf("Factorial(%d) = %ld\n", n, result);
            break;
        case 2:
            result = fibonacci(n);
            if (result < 0) {
                fprintf(stderr, "Overflow in fibonacci\n");
                return 1;
            }
            printf("Fibonacci(%d) = %ld\n", n, result);
            break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}