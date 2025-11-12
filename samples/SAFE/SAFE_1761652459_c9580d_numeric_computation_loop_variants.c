//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT_LEN 32

int is_valid_number(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    int has_digit = 0;
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        has_digit = 1;
    }
    return has_digit;
}

long safe_strtol(const char *str, long min, long max) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return LONG_MIN;
    if (val < min || val > max) return LONG_MIN;
    return val;
}

int main(void) {
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
    
    n = safe_strtol(input, 1, 1000);
    if (n == LONG_MIN) {
        fprintf(stderr, "Number out of range\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum1 = 0;
    long i = 1;
    while (i <= n) {
        if (i > 0 && i <= n) {
            long square = i * i;
            if (square > LONG_MAX - sum1) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum1 += square;
        }
        i++;
    }
    printf("While loop sum: %ld\n", sum1);
    
    long sum2 = 0;
    for (long j = 1; j <= n; j++) {
        if (j > 0 && j <= n) {
            long square = j * j;
            if (square > LONG_MAX - sum2) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum2 += square;
        }
    }
    printf("For loop sum: %ld\n", sum2);
    
    long sum3 = 0;
    long k = 1;
    do {
        if (k > 0 && k <= n) {
            long square = k * k;
            if (square > LONG_MAX - sum3) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum3 += square;
        }
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %ld\n", sum3);
    
    long sum4 = 0;
    long m = 1;
    loop_start:
    if (m <= n) {
        if (m > 0 && m <= n) {
            long square = m * m;
            if (square > LONG_MAX - sum4) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum4 += square;
        }
        m++;
        goto loop_start;
    }
    printf("Goto loop sum: %ld\n", sum4);
    
    double exact = (double)n * (n + 1) * (2 * n + 1) / 6.0;
    printf("Exact formula result: %.0f\n", exact);
    
    return 0;
}