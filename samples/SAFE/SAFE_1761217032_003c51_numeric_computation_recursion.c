//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
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
    
    int a = fibonacci(n - 1);
    int b = fibonacci(n - 2);
    
    if (a == -1 || b == -1) return -1;
    if (a > INT_MAX - b) return -1;
    
    return a + b;
}

int main() {
    int choice, num;
    
    printf("Numeric Computation Program\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &num) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num < 0) {
        printf("Number must be non-negative\n");
        return 1;
    }
    
    if (choice == 1) {
        if (num > 20) {
            printf("Number too large for factorial calculation\n");
            return 1;
        }
        
        long long result = factorial(num);
        if (result == -1) {
            printf("Overflow occurred in factorial calculation\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", num, result);
    } else {
        if (num > 46) {
            printf("Number too large for Fibonacci calculation\n");
            return 1;
        }
        
        int result = fibonacci(num);
        if (result == -1) {
            printf("Overflow occurred in Fibonacci calculation\n");
            return 1;
        }
        printf("Fibonacci number at position %d is %d\n", num, result);
    }
    
    return 0;
}