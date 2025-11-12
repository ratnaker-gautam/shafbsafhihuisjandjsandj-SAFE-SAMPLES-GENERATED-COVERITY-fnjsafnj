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
    int has_digit = 0;
    while (*str) {
        if (*str < '0' || *str > '9') return 0;
        has_digit = 1;
        str++;
    }
    return has_digit;
}

long parse_long(const char *str) {
    long result = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        if (result > LONG_MAX / 10) return sign == 1 ? LONG_MAX : LONG_MIN;
        result *= 10;
        int digit = *str - '0';
        if (result > LONG_MAX - digit) return sign == 1 ? LONG_MAX : LONG_MIN;
        result += digit;
        str++;
    }
    return sign * result;
}

int main() {
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
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum_for = 0;
    long i = 1;
    for (; i <= n; i++) {
        if (i > LONG_MAX / i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long square = i * i;
        if (sum_for > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += square;
    }
    printf("For loop sum: %ld\n", sum_for);
    
    long sum_while = 0;
    long j = 1;
    while (j <= n) {
        if (j > LONG_MAX / j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long square = j * j;
        if (sum_while > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += square;
        j++;
    }
    printf("While loop sum: %ld\n", sum_while);
    
    long sum_do_while = 0;
    long k = 1;
    if (n >= 1) {
        do {
            if (k > LONG_MAX / k) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            long square = k * k;
            if (sum_do_while > LONG_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_do_while += square;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %ld\n", sum_do_while);
    
    double sqrt_sum_for = sqrt((double)sum_for);
    double sqrt_sum_while = sqrt((double)sum_while);
    double sqrt_sum_do_while = sqrt((double)sum_do_while);
    
    printf("\nSquare roots of sums:\n");
    printf("For loop sqrt: %.6f\n", sqrt_sum_for);
    printf("While loop sqrt: %.6f\n", sqrt_sum_while);
    printf("Do-while loop sqrt: %.6f\n", sqrt_sum_do_while);
    
    return 0;
}