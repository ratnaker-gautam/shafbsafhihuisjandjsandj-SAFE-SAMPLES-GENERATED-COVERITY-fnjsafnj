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
        if (result > LONG_MAX / 10) return sign == 1 ? LONG_MAX : LONG_MIN;
        result *= 10;
        int digit = str[i] - '0';
        if (result > LONG_MAX - digit) return sign == 1 ? LONG_MAX : LONG_MIN;
        result += digit;
        i++;
    }
    return sign * result;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
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
    long n = parse_long(input);
    if (n <= 0) {
        fprintf(stderr, "Number must be positive\n");
        return 1;
    }
    if (n > 1000000) {
        fprintf(stderr, "Number too large\n");
        return 1;
    }
    printf("Computing sum of squares from 1 to %ld\n", n);
    long sum = 0;
    long i = 1;
    while (i <= n) {
        if (i > LONG_MAX / i) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        long square = i * i;
        if (sum > LONG_MAX - square) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        sum += square;
        i++;
    }
    printf("Sum computed with while loop: %ld\n", sum);
    sum = 0;
    for (i = 1; i <= n; i++) {
        if (i > LONG_MAX / i) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        long square = i * i;
        if (sum > LONG_MAX - square) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        sum += square;
    }
    printf("Sum computed with for loop: %ld\n", sum);
    sum = 0;
    i = 1;
    do {
        if (i > LONG_MAX / i) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        long square = i * i;
        if (sum > LONG_MAX - square) {
            fprintf(stderr, "Overflow detected\n");
            return 1;
        }
        sum += square;
        i++;
    } while (i <= n);
    printf("Sum computed with do-while loop: %ld\n", sum);
    return 0;
}