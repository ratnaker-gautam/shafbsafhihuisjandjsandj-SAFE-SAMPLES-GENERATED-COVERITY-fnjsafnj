//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    long fib1 = fibonacci(n - 1);
    long fib2 = fibonacci(n - 2);
    if (fib1 == -1 || fib2 == -1) return -1;
    long result = fib1 + fib2;
    if (result < 0) return -1;
    return result;
}

int read_positive_int(const char* prompt, int max_value) {
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
    if (value < 0 || value > max_value) {
        return -1;
    }
    return (int)value;
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("=======================\n\n");
    
    int choice = read_positive_int("Choose operation:\n1. Factorial\n2. Fibonacci\nEnter choice (1-2): ", 2);
    if (choice == -1) {
        printf("Invalid choice input.\n");
        return 1;
    }
    
    int max_n = (choice == 1) ? 20 : 40;
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter n (0-%d): ", max_n);
    
    int n = read_positive_int(prompt, max_n);
    if (n == -1) {
        printf("Invalid n input.\n");
        return 1;
    }
    
    long result;
    if (choice == 1) {
        result = factorial(n);
        if (result == -1) {
            printf("Error: Factorial computation failed (overflow or invalid input).\n");
            return 1;
        }
        printf("Factorial(%d) = %ld\n", n, result);
    } else {
        result = fibonacci(n);
        if (result == -1) {
            printf("Error: Fibonacci computation failed (overflow or invalid input).\n");
            return 1;
        }
        printf("Fibonacci(%d) = %ld\n", n, result);
    }
    
    return 0;
}