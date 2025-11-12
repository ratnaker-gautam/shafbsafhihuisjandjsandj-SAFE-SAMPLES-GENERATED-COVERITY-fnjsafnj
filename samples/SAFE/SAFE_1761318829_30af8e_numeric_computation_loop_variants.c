//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

int main(void) {
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
    } else if (len == sizeof(input) - 1) {
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
    
    printf("1. For loop: ");
    long sum_for = 0;
    for (long i = 1; i <= n; i++) {
        if (i > 0 && sum_for > LONG_MAX - i * i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += i * i;
    }
    printf("%ld\n", sum_for);
    
    printf("2. While loop: ");
    long sum_while = 0;
    long j = 1;
    while (j <= n) {
        if (j > 0 && sum_while > LONG_MAX - j * j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += j * j;
        j++;
    }
    printf("%ld\n", sum_while);
    
    printf("3. Do-while loop: ");
    long sum_do_while = 0;
    long k = 1;
    if (n >= 1) {
        do {
            if (k > 0 && sum_do_while > LONG_MAX - k * k) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_do_while += k * k;
            k++;
        } while (k <= n);
    }
    printf("%ld\n", sum_do_while);
    
    printf("4. Formula verification: ");
    long sum_formula = n * (n + 1) * (2 * n + 1) / 6;
    printf("%ld\n", sum_formula);
    
    if (sum_for == sum_while && sum_while == sum_do_while && sum_do_while == sum_formula) {
        printf("All methods agree: sum = %ld\n", sum_for);
    } else {
        printf("Discrepancy detected between methods\n");
        return 1;
    }
    
    return 0;
}