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
    int has_digit = 0;
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            has_digit = 1;
            i++;
        } else {
            return 0;
        }
    }
    return has_digit;
}

long safe_strtol(const char *str, int *valid) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        *valid = 0;
        return 0;
    }
    if (val == LONG_MAX || val == LONG_MIN) {
        *valid = 0;
        return 0;
    }
    *valid = 1;
    return val;
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    long n;
    int valid;
    
    printf("Enter an integer (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_number(input)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    n = safe_strtol(input, &valid);
    if (!valid) {
        fprintf(stderr, "Number out of range\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Number must be between 1 and 100\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum1 = 0;
    long i = 1;
    while (i <= n) {
        if (i > 0 && i <= 100) {
            long square = i * i;
            if (square > 0 && sum1 <= LONG_MAX - square) {
                sum1 += square;
            } else {
                fprintf(stderr, "Overflow detected\n");
                return 1;
            }
        }
        i++;
    }
    printf("While loop sum: %ld\n", sum1);
    
    long sum2 = 0;
    for (long j = 1; j <= n; j++) {
        if (j > 0 && j <= 100) {
            long square = j * j;
            if (square > 0 && sum2 <= LONG_MAX - square) {
                sum2 += square;
            } else {
                fprintf(stderr, "Overflow detected\n");
                return 1;
            }
        }
    }
    printf("For loop sum: %ld\n", sum2);
    
    long sum3 = 0;
    long k = 1;
    do {
        if (k > 0 && k <= 100) {
            long square = k * k;
            if (square > 0 && sum3 <= LONG_MAX - square) {
                sum3 += square;
            } else {
                fprintf(stderr, "Overflow detected\n");
                return 1;
            }
        }
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %ld\n", sum3);
    
    long sum4 = 0;
    long m = n;
    while (m >= 1) {
        if (m > 0 && m <= 100) {
            long square = m * m;
            if (square > 0 && sum4 <= LONG_MAX - square) {
                sum4 += square;
            } else {
                fprintf(stderr, "Overflow detected\n");
                return 1;
            }
        }
        m--;
    }
    printf("Reverse while loop sum: %ld\n", sum4);
    
    if (sum1 == sum2 && sum2 == sum3 && sum3 == sum4) {
        printf("\nAll loop variants produced the same result: %ld\n", sum1);
    } else {
        fprintf(stderr, "Error: Loop variants produced different results\n");
        return 1;
    }
    
    return 0;
}