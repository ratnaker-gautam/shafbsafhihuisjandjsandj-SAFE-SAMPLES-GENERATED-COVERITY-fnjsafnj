//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    return n * factorial(n - 1);
}

int read_valid_int(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long temp = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (temp < INT_MIN || temp > INT_MAX) return 0;
    *value = (int)temp;
    return 1;
}

int main() {
    int number;
    printf("Enter an integer (0-20) to compute factorial: ");
    if (!read_valid_int(&number)) {
        printf("Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    if (number < 0 || number > 20) {
        printf("Input out of range. Please enter a number between 0 and 20.\n");
        return 1;
    }
    long long result = factorial(number);
    if (result == -1) {
        printf("Error computing factorial.\n");
        return 1;
    }
    printf("Factorial of %d is %lld\n", number, result);
    return 0;
}