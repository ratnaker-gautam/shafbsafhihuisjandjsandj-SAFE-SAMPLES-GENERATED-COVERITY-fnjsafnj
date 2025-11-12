//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
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
    int choice, number;
    
    printf("Numeric Computation Menu:\n");
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
    
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (number < 0) {
        printf("Number must be non-negative.\n");
        return 1;
    }
    
    if (choice == 1) {
        long long result = factorial(number);
        if (result == -1) {
            printf("Error: Computation failed or overflow occurred.\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", number, result);
    } else {
        long long result = fibonacci(number);
        if (result == -1) {
            printf("Error: Computation failed or overflow occurred.\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %lld\n", number, result);
    }
    
    return 0;
}