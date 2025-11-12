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

int read_positive_int(const char* prompt, int max) {
    printf("%s", prompt);
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char* endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < 0 || value > max) {
        return -1;
    }
    return (int)value;
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    
    int choice = read_positive_int("Choose operation (1-2): ", 2);
    if (choice == -1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1) {
        int n = read_positive_int("Enter n for factorial (0-20): ", 20);
        if (n == -1) {
            printf("Invalid input\n");
            return 1;
        }
        long long result = factorial(n);
        if (result == -1) {
            printf("Computation error\n");
            return 1;
        }
        printf("Factorial(%d) = %lld\n", n, result);
    } else {
        int n = read_positive_int("Enter n for Fibonacci (0-92): ", 92);
        if (n == -1) {
            printf("Invalid input\n");
            return 1;
        }
        long long result = fibonacci(n);
        if (result == -1) {
            printf("Computation error\n");
            return 1;
        }
        printf("Fibonacci(%d) = %lld\n", n, result);
    }
    
    return 0;
}