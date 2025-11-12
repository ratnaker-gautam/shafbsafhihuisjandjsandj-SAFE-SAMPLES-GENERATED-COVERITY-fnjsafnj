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

int read_positive_int(const char* prompt, int max_value) {
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
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < 0) {
            printf("Please enter a non-negative integer.\n");
            continue;
        }
        
        if (value > max_value) {
            printf("Value too large. Maximum allowed is %d.\n", max_value);
            continue;
        }
        
        break;
    }
    
    return value;
}

int main() {
    printf("Factorial Calculator\n");
    printf("===================\n");
    
    int max_n = 20;
    int n = read_positive_int("Enter a non-negative integer (0-20): ", max_n);
    
    if (n < 0) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    long long result = factorial(n);
    
    if (result == -1) {
        printf("Error: Integer overflow occurred.\n");
        return 1;
    }
    
    printf("%d! = %lld\n", n, result);
    
    printf("\nFirst %d factorials:\n", n);
    for (int i = 0; i <= n; i++) {
        long long fact = factorial(i);
        if (fact != -1) {
            printf("%d! = %lld\n", i, fact);
        } else {
            printf("%d! = overflow\n", i);
        }
    }
    
    return 0;
}