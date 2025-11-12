//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    if (str[0] == '\0') return 0;
    
    int has_digits = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            has_digits = 1;
        } else if (i == 0 && (str[i] == '-' || str[i] == '+')) {
            continue;
        } else {
            return 0;
        }
    }
    return has_digits;
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
        if (result > LLONG_MAX / 10) return LLONG_MAX;
        result = result * 10;
        
        int digit = str[i] - '0';
        if (result > LLONG_MAX - digit) return LLONG_MAX;
        result = result + digit;
    }
    
    return negative ? -result : result;
}

int is_prime(long long n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}

long long sum_primes_up_to(long long limit) {
    if (limit < 2) return 0;
    
    long long sum = 2;
    for (long long n = 3; n <= limit; n += 2) {
        if (is_prime(n)) {
            if (sum > LLONG_MAX - n) {
                return LLONG_MAX;
            }
            sum += n;
        }
    }
    return sum;
}

int main() {
    char input[64];
    printf("Enter a positive integer (max %lld): ", (long long)MAX_ITERATIONS);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0' && input[i] != '\n') i++;
    if (input[i] == '\n') input[i] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: must be a positive integer\n");
        return 1;
    }
    
    long long number = parse_long_long(input);
    if (number <= 0) {
        fprintf(stderr, "Number must be positive\n");
        return 1;
    }
    
    if (number > MAX_ITERATIONS) {
        fprintf(stderr, "Number too large (max %d)\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Computing sum of primes up to %lld...\n", number);
    
    long long result = sum_primes_up_to(number);
    
    if (result == LLONG_MAX) {
        fprintf(stderr, "Overflow occurred during computation\n");
        return 1;
    }
    
    printf("Sum of primes up to %lld: %lld\n", number, result);
    
    return 0;
}