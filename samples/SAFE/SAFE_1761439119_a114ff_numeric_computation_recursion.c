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

int read_positive_int(const char* prompt, int max_value) {
    char buffer[32];
    int value;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }
    
    if (value < 0 || value > max_value) {
        return -1;
    }
    
    return value;
}

int main() {
    int choice;
    
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    
    choice = read_positive_int("Choose operation (1-2): ", 2);
    if (choice == -1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1) {
        int n = read_positive_int("Enter number (0-20): ", 20);
        if (n == -1) {
            printf("Invalid input\n");
            return 1;
        }
        
        long long result = factorial(n);
        if (result == -1) {
            printf("Computation error\n");
            return 1;
        }
        
        printf("Factorial of %d is %lld\n", n, result);
    } else {
        int n = read_positive_int("Enter position (0-46): ", 46);
        if (n == -1) {
            printf("Invalid input\n");
            return 1;
        }
        
        int result = fibonacci(n);
        if (result == -1) {
            printf("Computation error\n");
            return 1;
        }
        
        printf("Fibonacci number at position %d is %d\n", n, result);
    }
    
    return 0;
}