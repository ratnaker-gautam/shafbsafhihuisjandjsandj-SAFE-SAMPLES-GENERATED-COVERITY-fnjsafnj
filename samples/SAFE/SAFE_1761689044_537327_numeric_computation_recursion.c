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

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        break;
    }
    return value;
}

int main(void) {
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    
    int choice = get_valid_int("Choose operation (1-2): ", 1, 2);
    if (choice == -1) {
        return 1;
    }
    
    if (choice == 1) {
        int n = get_valid_int("Enter n for factorial (0-20): ", 0, 20);
        if (n == -1) {
            return 1;
        }
        long result = recursive_factorial(n);
        if (result == -1) {
            printf("Error: Computation failed.\n");
            return 1;
        }
        printf("Factorial(%d) = %ld\n", n, result);
    } else {
        int n = get_valid_int("Enter n for Fibonacci (0-46): ", 0, 46);
        if (n == -1) {
            return 1;
        }
        long result = recursive_fibonacci(n);
        if (result == -1) {
            printf("Error: Computation failed.\n");
            return 1;
        }
        printf("Fibonacci(%d) = %ld\n", n, result);
    }
    
    return 0;
}