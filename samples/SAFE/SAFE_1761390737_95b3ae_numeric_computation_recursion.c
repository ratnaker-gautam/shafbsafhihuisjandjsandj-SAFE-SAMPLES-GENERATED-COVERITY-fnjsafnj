//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 46) return -1;
    long a = fibonacci(n - 1);
    long b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    long result = a + b;
    if (result < 0) return -1;
    return result;
}

int read_positive_int(const char* prompt, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < 0) {
            printf("Please enter a non-negative number.\n");
            continue;
        }
        if (value > max) {
            printf("Value too large. Maximum allowed is %d.\n", max);
            continue;
        }
        return value;
    }
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    
    int choice;
    while (1) {
        printf("Choose operation (1 or 2): ");
        char buffer[32];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 1;
        }
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter 1 or 2.\n");
            continue;
        }
        if (choice == 1 || choice == 2) {
            break;
        }
        printf("Please enter 1 or 2.\n");
    }
    
    if (choice == 1) {
        int n = read_positive_int("Enter number for factorial (0-20): ", 20);
        if (n == -1) {
            return 1;
        }
        long result = factorial(n);
        if (result == -1) {
            printf("Error: Computation failed or overflow occurred.\n");
        } else {
            printf("Factorial of %d is %ld\n", n, result);
        }
    } else {
        int n = read_positive_int("Enter Fibonacci position (0-46): ", 46);
        if (n == -1) {
            return 1;
        }
        long result = fibonacci(n);
        if (result == -1) {
            printf("Error: Computation failed or overflow occurred.\n");
        } else {
            printf("Fibonacci number at position %d is %ld\n", n, result);
        }
    }
    
    return 0;
}