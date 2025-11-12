//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: numeric_computation
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

long long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    
    if (a == -1 || b == -1) return -1;
    if (a > LLONG_MAX - b) return -1;
    
    return a + b;
}

int get_valid_input(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("=======================\n\n");
    
    int choice;
    do {
        printf("Choose operation:\n");
        printf("1. Calculate factorial\n");
        printf("2. Calculate Fibonacci number\n");
        printf("3. Exit\n");
        
        choice = get_valid_input("Enter choice (1-3): ", 1, 3);
        
        if (choice == 1) {
            int n = get_valid_input("Enter number for factorial (0-20): ", 0, 20);
            long long result = factorial(n);
            if (result == -1) {
                printf("Error: Overflow occurred.\n");
            } else {
                printf("Factorial of %d is %lld\n\n", n, result);
            }
        } else if (choice == 2) {
            int n = get_valid_input("Enter Fibonacci position (0-50): ", 0, 50);
            long long result = fibonacci(n);
            if (result == -1) {
                printf("Error: Overflow occurred.\n");
            } else {
                printf("Fibonacci number at position %d is %lld\n\n", n, result);
            }
        }
    } while (choice != 3);
    
    printf("Goodbye!\n");
    return 0;
}