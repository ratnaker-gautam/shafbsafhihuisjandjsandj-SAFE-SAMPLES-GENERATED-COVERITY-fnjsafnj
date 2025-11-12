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

int read_positive_int(int max_value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (value < 0 || value > max_value) return -1;
    
    return (int)value;
}

int main() {
    printf("Factorial Calculator\n");
    printf("Enter a positive integer (0-20): ");
    
    int n = read_positive_int(20);
    if (n == -1) {
        printf("Invalid input. Please enter an integer between 0 and 20.\n");
        return 1;
    }
    
    long long result = factorial(n);
    if (result == -1) {
        printf("Error: Integer overflow occurred.\n");
        return 1;
    }
    
    printf("%d! = %lld\n", n, result);
    
    return 0;
}