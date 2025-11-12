//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT_LEN 32

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    int dot_count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

double parse_double(const char *str) {
    double result = 0.0;
    double fraction = 0.1;
    int decimal_found = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        if (str[i] == '.') {
            decimal_found = 1;
        } else {
            if (!decimal_found) {
                result = result * 10.0 + (str[i] - '0');
            } else {
                result += (str[i] - '0') * fraction;
                fraction *= 0.1;
            }
        }
        i++;
    }
    return result;
}

double compute_sqrt_newton(double n) {
    if (n < 0.0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int i = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        i++;
    } while (fabs(x - prev) > 1e-10 && i < MAX_ITERATIONS);
    
    return x;
}

double compute_sqrt_binary(double n) {
    if (n < 0.0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double low = 0.0, high = n;
    if (n < 1.0) high = 1.0;
    
    double mid;
    int i = 0;
    
    while (i < MAX_ITERATIONS) {
        mid = (low + high) / 2.0;
        double sq = mid * mid;
        
        if (fabs(sq - n) < 1e-10) break;
        
        if (sq < n) {
            low = mid;
        } else {
            high = mid;
        }
        i++;
    }
    
    return mid;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a positive number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_number(input)) {
        fprintf(stderr, "Invalid input: must be a positive number\n");
        return 1;
    }
    
    double number = parse_double(input);
    if (number < 0.0) {
        fprintf(stderr, "Number must be positive\n");
        return 1;
    }
    
    printf("Computing square root of %.6f using different methods:\n", number);
    
    double result1 = compute_sqrt_newton(number);
    double result2 = compute_sqrt_binary(number);
    double reference = sqrt(number);
    
    printf("Newton's method:    %.10f\n", result1);
    printf("Binary search:      %.10f\n", result2);
    printf("Library sqrt:       %.10f\n", reference);
    printf("Newton error:       %.2e\n", fabs(result1 - reference));
    printf("Binary error:       %.2e\n", fabs(result2 - reference));
    
    return 0;
}