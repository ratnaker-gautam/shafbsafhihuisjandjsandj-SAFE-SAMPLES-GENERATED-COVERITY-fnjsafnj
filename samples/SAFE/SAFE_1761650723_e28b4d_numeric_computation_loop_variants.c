//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 32

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    if (*str == '-' || *str == '+') str++;
    int has_digits = 0;
    while (*str) {
        if (*str < '0' || *str > '9') return 0;
        has_digits = 1;
        str++;
    }
    return has_digits;
}

long parse_long(const char *str) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return LONG_MAX;
    return val;
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    long n;
    
    printf("Enter a positive integer (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_number(input)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    n = parse_long(input);
    if (n == LONG_MAX || n < 1 || n > 1000) {
        fprintf(stderr, "Number out of range (1-1000)\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum1 = 0;
    long i = 1;
    while (i <= n) {
        if (i > LONG_MAX / i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long square = i * i;
        if (sum1 > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum1 += square;
        i++;
    }
    printf("While loop sum: %ld\n", sum1);
    
    long sum2 = 0;
    for (long j = 1; j <= n; j++) {
        if (j > LONG_MAX / j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long square = j * j;
        if (sum2 > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum2 += square;
    }
    printf("For loop sum: %ld\n", sum2);
    
    long sum3 = 0;
    long k = 1;
    do {
        if (k > LONG_MAX / k) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long square = k * k;
        if (sum3 > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum3 += square;
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %ld\n", sum3);
    
    long expected = n * (n + 1) * (2 * n + 1) / 6;
    printf("Expected sum (formula): %ld\n", expected);
    
    if (sum1 == sum2 && sum2 == sum3 && sum3 == expected) {
        printf("All loop variants produced correct result!\n");
    } else {
        printf("Results mismatch detected!\n");
        return 1;
    }
    
    return 0;
}