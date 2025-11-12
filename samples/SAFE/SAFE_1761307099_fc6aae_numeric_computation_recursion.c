//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    long long result = n * factorial(n - 1);
    if (result < 0) {
        return -1;
    }
    return result;
}

long long fibonacci(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    if (a < 0 || b < 0) {
        return -1;
    }
    long long result = a + b;
    if (result < 0) {
        return -1;
    }
    return result;
}

int main() {
    int choice;
    printf("Choose operation:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n");
        return 1;
    }
    int number;
    printf("Enter a non-negative integer (0-20): ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (number < 0 || number > 20) {
        printf("Number out of range.\n");
        return 1;
    }
    if (choice == 1) {
        long long result = factorial(number);
        if (result < 0) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Factorial of %d is %lld\n", number, result);
    } else {
        long long result = fibonacci(number);
        if (result < 0) {
            printf("Overflow occurred.\n");
            return 1;
        }
        printf("Fibonacci number %d is %lld\n", number, result);
    }
    return 0;
}