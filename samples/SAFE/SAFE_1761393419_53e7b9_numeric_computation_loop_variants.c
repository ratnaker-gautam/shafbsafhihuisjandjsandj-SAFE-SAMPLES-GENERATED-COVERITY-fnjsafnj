//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int validate_input(const char* input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    
    int has_digit = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (i == 0 && input[i] == '-') continue;
        if (input[i] < '0' || input[i] > '9') return 0;
        has_digit = 1;
    }
    return has_digit;
}

long parse_long(const char* input) {
    long result = 0;
    int sign = 1;
    int i = 0;
    
    if (input[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    while (input[i] != '\0') {
        if (result > LONG_MAX / 10) return sign == 1 ? LONG_MAX : LONG_MIN;
        result = result * 10;
        
        int digit = input[i] - '0';
        if (result > LONG_MAX - digit) return sign == 1 ? LONG_MAX : LONG_MIN;
        result = result + digit;
        i++;
    }
    
    return result * sign;
}

int main(void) {
    char input[MAX_INPUT + 1];
    long number;
    
    printf("Enter a positive integer: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: must be a positive integer\n");
        return 1;
    }
    
    number = parse_long(input);
    if (number <= 0) {
        fprintf(stderr, "Number must be positive\n");
        return 1;
    }
    
    if (number > 1000000) {
        fprintf(stderr, "Number too large (max: 1000000)\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum_for = 0;
    for (long i = 1; i <= number; i++) {
        if (i > 0 && sum_for > LONG_MAX - i * i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += i * i;
    }
    printf("For loop sum: %ld\n", sum_for);
    
    long sum_while = 0;
    long j = 1;
    while (j <= number) {
        if (j > 0 && sum_while > LONG_MAX - j * j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += j * j;
        j++;
    }
    printf("While loop sum: %ld\n", sum_while);
    
    long sum_do_while = 0;
    long k = 1;
    if (number >= 1) {
        do {
            if (k > 0 && sum_do_while > LONG_MAX - k * k) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_do_while += k * k;
            k++;
        } while (k <= number);
    }
    printf("Do-while loop sum: %ld\n", sum_do_while);
    
    double exact_formula = (double)number * (number + 1) * (2 * number + 1) / 6.0;
    printf("Exact formula result: %.0f\n", exact_formula);
    
    double difference = fabs((double)sum_for - exact_formula);
    printf("Difference between methods: %.6f\n", difference);
    
    return 0;
}