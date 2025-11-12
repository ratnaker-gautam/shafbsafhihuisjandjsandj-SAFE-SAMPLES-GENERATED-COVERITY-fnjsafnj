//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long recursive_factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    long result = n * recursive_factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long recursive_fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 46) return -1;
    long a = recursive_fibonacci(n - 1);
    long b = recursive_fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    if (a > LONG_MAX - b) return -1;
    return a + b;
}

int main() {
    int choice, number;
    long result;
    
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("Enter choice (1-2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter number: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice == 1) {
        result = recursive_factorial(number);
        if (result == -1) {
            printf("Error: Invalid input or overflow\n");
            return 1;
        }
        printf("Factorial of %d is %ld\n", number, result);
    } else {
        result = recursive_fibonacci(number);
        if (result == -1) {
            printf("Error: Invalid input or overflow\n");
            return 1;
        }
        printf("Fibonacci number %d is %ld\n", number, result);
    }
    
    return 0;
}