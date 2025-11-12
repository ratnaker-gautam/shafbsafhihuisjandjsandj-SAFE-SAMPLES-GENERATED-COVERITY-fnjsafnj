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

long long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    long long result = a + b;
    if (result < a) return -1;
    return result;
}

int main(void) {
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
        fprintf(stderr, "Invalid choice format\n");
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
        fprintf(stderr, "Invalid n format\n");
        return 1;
    }
    
    if (n < 0 || n > 20) {
        fprintf(stderr, "n must be between 0 and 20\n");
        return 1;
    }
    
    long long result;
    
    if (choice == 1) {
        result = factorial(n);
        if (result == -1) {
            fprintf(stderr, "Factorial computation failed\n");
            return 1;
        }
        printf("Factorial(%d) = %lld\n", n, result);
    } else {
        result = fibonacci(n);
        if (result == -1) {
            fprintf(stderr, "Fibonacci computation failed\n");
            return 1;
        }
        printf("Fibonacci(%d) = %lld\n", n, result);
    }
    
    return 0;
}