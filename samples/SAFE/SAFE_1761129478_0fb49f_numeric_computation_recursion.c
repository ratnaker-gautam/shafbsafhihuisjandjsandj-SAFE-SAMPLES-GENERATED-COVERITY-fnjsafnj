//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    long long result = n * factorial(n - 1);
    if (result < 0 || result / n != factorial(n - 1)) {
        return -1;
    }
    return result;
}

int main() {
    int number;
    printf("Enter a non-negative integer (0-20): ");
    
    if (scanf("%d", &number) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }
    
    if (number < 0 || number > 20) {
        printf("Number must be between 0 and 20.\n");
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