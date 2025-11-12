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

int fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 46) return -1;
    int a = fibonacci(n - 1);
    int b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    if (a > INT_MAX - b) return -1;
    return a + b;
}

int read_positive_int(const char* prompt) {
    printf("%s", prompt);
    int value;
    if (scanf("%d", &value) != 1) {
        return -1;
    }
    if (value < 0) {
        return -1;
    }
    return value;
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    
    int choice;
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice == 1) {
        int n = read_positive_int("Enter n (0-20): ");
        if (n == -1) {
            printf("Invalid input\n");
            return 1;
        }
        long long result = factorial(n);
        if (result == -1) {
            printf("Computation error or overflow\n");
            return 1;
        }
        printf("Factorial(%d) = %lld\n", n, result);
    } else if (choice == 2) {
        int n = read_positive_int("Enter n (0-46): ");
        if (n == -1) {
            printf("Invalid input\n");
            return 1;
        }
        int result = fibonacci(n);
        if (result == -1) {
            printf("Computation error or overflow\n");
            return 1;
        }
        printf("Fibonacci(%d) = %d\n", n, result);
    } else {
        printf("Invalid choice\n");
        return 1;
    }
    
    return 0;
}