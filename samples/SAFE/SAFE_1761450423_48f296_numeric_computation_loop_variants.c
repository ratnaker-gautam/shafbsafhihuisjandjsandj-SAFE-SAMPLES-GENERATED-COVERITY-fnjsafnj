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
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        has_digit = 1;
    }
    return has_digit;
}

long parse_long(const char *str) {
    long result = 0;
    int sign = 1;
    int i = 0;
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    for (; str[i] != '\0'; i++) {
        if (result > LONG_MAX / 10) return sign == 1 ? LONG_MAX : LONG_MIN;
        result *= 10;
        int digit = str[i] - '0';
        if (result > LONG_MAX - digit) return sign == 1 ? LONG_MAX : LONG_MIN;
        result += digit;
    }
    return sign * result;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    long n = 0;
    
    printf("Enter a positive integer (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    n = parse_long(input);
    if (n <= 0 || n > 1000) {
        fprintf(stderr, "Number out of range (1-1000)\n");
        return 1;
    }
    
    printf("Computing sum of reciprocals 1/1 + 1/2 + ... + 1/%ld\n", n);
    
    double sum_for = 0.0;
    long i;
    for (i = 1; i <= n; i++) {
        if (i == 0) continue;
        sum_for += 1.0 / (double)i;
    }
    
    double sum_while = 0.0;
    long j = 1;
    while (j <= n) {
        if (j != 0) {
            sum_while += 1.0 / (double)j;
        }
        j++;
    }
    
    double sum_do_while = 0.0;
    long k = 1;
    if (n >= 1) {
        do {
            if (k != 0) {
                sum_do_while += 1.0 / (double)k;
            }
            k++;
        } while (k <= n);
    }
    
    printf("For loop result: %.15f\n", sum_for);
    printf("While loop result: %.15f\n", sum_while);
    printf("Do-while loop result: %.15f\n", sum_do_while);
    
    double diff1 = fabs(sum_for - sum_while);
    double diff2 = fabs(sum_while - sum_do_while);
    double tolerance = 1e-12;
    
    if (diff1 < tolerance && diff2 < tolerance) {
        printf("All loop variants produced consistent results\n");
    } else {
        printf("Warning: Loop variants produced different results\n");
    }
    
    return 0;
}