//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    if (str[0] == '\0') return 0;
    
    int has_digit = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            has_digit = 1;
        } else if (str[i] != '-' && str[i] != '+') {
            return 0;
        }
    }
    return has_digit;
}

long long parse_long_long(const char* str) {
    long long result = 0;
    int negative = 0;
    int i = 0;
    
    if (str[0] == '-') {
        negative = 1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            if (result > LLONG_MAX / 10) return LLONG_MAX;
            result = result * 10;
            int digit = str[i] - '0';
            if (result > LLONG_MAX - digit) return LLONG_MAX;
            result = result + digit;
        }
    }
    
    return negative ? -result : result;
}

int main() {
    char input[256];
    long long n;
    
    printf("Enter a positive integer (1-%d): ", MAX_ITERATIONS);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0' && input[i] != '\n') i++;
    if (input[i] == '\n') input[i] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    n = parse_long_long(input);
    
    if (n <= 0) {
        fprintf(stderr, "Number must be positive\n");
        return 1;
    }
    
    if (n > MAX_ITERATIONS) {
        fprintf(stderr, "Number too large (max: %d)\n", MAX_ITERATIONS);
        return 1;
    }
    
    long long sum = 0;
    for (long long i = 1; i <= n; i++) {
        if (sum > LLONG_MAX - (i * i)) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum += i * i;
    }
    
    double pi_approximation = 0.0;
    for (long long k = 0; k < n; k++) {
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2.0 * k + 1.0);
        pi_approximation += term;
    }
    pi_approximation *= 4.0;
    
    printf("Sum of squares from 1 to %lld: %lld\n", n, sum);
    printf("Pi approximation using %lld terms: %.15f\n", n, pi_approximation);
    
    return 0;
}