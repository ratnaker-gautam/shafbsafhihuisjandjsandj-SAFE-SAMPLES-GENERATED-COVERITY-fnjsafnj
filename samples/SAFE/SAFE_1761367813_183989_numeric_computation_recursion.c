//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

int read_positive_int(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long temp = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (temp < 0 || temp > INT_MAX) return 0;
    *value = (int)temp;
    return 1;
}

int main(void) {
    printf("Enter a non-negative integer (0-20): ");
    int n;
    if (!read_positive_int(&n)) {
        printf("Invalid input. Must be integer 0-20.\n");
        return 1;
    }
    if (n > 20) {
        printf("Input too large. Maximum is 20.\n");
        return 1;
    }
    long long result = factorial(n);
    if (result == -1) {
        printf("Error: Overflow in computation.\n");
        return 1;
    }
    printf("Factorial of %d is %lld\n", n, result);
    return 0;
}