//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    long long result = n * factorial(n - 1);
    if (result < 0 || result / n != factorial(n - 1)) {
        return -1;
    }
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

int main(void) {
    int choice, number;
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
        fprintf(stderr, "Invalid choice format\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        fprintf(stderr, "Choice must be 1 or 2\n");
        return 1;
    }
    
    printf("Enter a non-negative integer: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &number) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (number < 0) {
        fprintf(stderr, "Number must be non-negative\n");
        return 1;
    }
    
    if (choice == 1) {
        long long result = factorial(number);
        if (result == -1) {
            fprintf(stderr, "Factorial computation failed: overflow or invalid input\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", number, result);
    } else {
        int result = fibonacci(number);
        if (result == -1) {
            fprintf(stderr, "Fibonacci computation failed: overflow or invalid input\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %d\n", number, result);
    }
    
    return 0;
}