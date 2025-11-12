//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    long long result = factorial(n - 1);
    if (result > LLONG_MAX / n) return -1;
    return result * n;
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

int main() {
    int choice, n;
    printf("Choose computation:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (n < 0) {
        printf("Number must be non-negative\n");
        return 1;
    }
    
    if (choice == 1) {
        long long result = factorial(n);
        if (result == -1) {
            printf("Overflow occurred\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", n, result);
    } else {
        int result = fibonacci(n);
        if (result == -1) {
            printf("Overflow occurred\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %d\n", n, result);
    }
    
    return 0;
}