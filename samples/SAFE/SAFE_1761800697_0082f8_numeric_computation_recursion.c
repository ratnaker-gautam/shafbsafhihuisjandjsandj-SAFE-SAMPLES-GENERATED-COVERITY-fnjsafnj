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

int read_positive_int(const char* prompt) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < 0) {
            printf("Please enter a non-negative integer.\n");
            continue;
        }
        
        if (value > 20) {
            printf("Value too large. Maximum allowed is 20.\n");
            continue;
        }
        
        return value;
    }
}

int main() {
    int number = read_positive_int("Enter a non-negative integer (0-20): ");
    
    if (number < 0) {
        fprintf(stderr, "Input error occurred.\n");
        return 1;
    }
    
    long long result = factorial(number);
    
    if (result == -1) {
        printf("Error: Integer overflow occurred.\n");
        return 1;
    }
    
    printf("Factorial of %d is %lld\n", number, result);
    
    return 0;
}