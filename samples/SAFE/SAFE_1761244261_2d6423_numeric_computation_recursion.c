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

int read_positive_int(int max_value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (val < 0 || val > max_value) return -1;
    return (int)val;
}

int main() {
    printf("Enter a positive integer (0-20) to compute factorial: ");
    int n = read_positive_int(20);
    if (n < 0) {
        printf("Invalid input. Please enter a valid integer between 0 and 20.\n");
        return 1;
    }
    long result = factorial(n);
    if (result < 0) {
        printf("Error: Integer overflow occurred.\n");
        return 1;
    }
    printf("Factorial of %d is %ld\n", n, result);
    return 0;
}