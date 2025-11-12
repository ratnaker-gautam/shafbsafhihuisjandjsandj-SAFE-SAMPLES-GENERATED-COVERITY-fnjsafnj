//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    
    long long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 92) return -1;
    
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    
    if (a == -1 || b == -1) return -1;
    if (a > LLONG_MAX - b) return -1;
    
    return a + b;
}

int main() {
    int choice, n;
    char buffer[100];
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter number: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice == 1) {
        long long result = factorial(n);
        if (result == -1) {
            printf("Error: Invalid input or overflow\n");
        } else {
            printf("Factorial(%d) = %lld\n", n, result);
        }
    } else if (choice == 2) {
        long long result = fibonacci(n);
        if (result == -1) {
            printf("Error: Invalid input or overflow\n");
        } else {
            printf("Fibonacci(%d) = %lld\n", n, result);
        }
    }
    
    return 0;
}