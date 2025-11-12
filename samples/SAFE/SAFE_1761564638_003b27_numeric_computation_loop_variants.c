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
    
    while (str[i] != '\0') {
        if (result > LONG_MAX / 10) return 0;
        result = result * 10;
        int digit = str[i] - '0';
        if (result > LONG_MAX - digit) return 0;
        result += digit;
        i++;
    }
    
    return result * sign;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    long n;
    
    printf("Enter a positive integer: ");
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
    if (n <= 0 || n > 1000000) {
        fprintf(stderr, "Number must be between 1 and 1000000\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum1 = 0;
    long i = 1;
    while (i <= n) {
        if (i > LONG_MAX / i) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        long square = i * i;
        if (sum1 > LONG_MAX - square) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        sum1 += square;
        i++;
    }
    printf("While loop sum: %ld\n", sum1);
    
    long sum2 = 0;
    for (long j = 1; j <= n; j++) {
        if (j > LONG_MAX / j) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        long square = j * j;
        if (sum2 > LONG_MAX - square) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        sum2 += square;
    }
    printf("For loop sum: %ld\n", sum2);
    
    long sum3 = 0;
    long k = 1;
    do {
        if (k > LONG_MAX / k) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        long square = k * k;
        if (sum3 > LONG_MAX - square) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        sum3 += square;
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %ld\n", sum3);
    
    double exact_sum = (double)n * (n + 1) * (2 * n + 1) / 6.0;
    printf("Exact formula result: %.0f\n", exact_sum);
    
    return 0;
}