//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    return result;
}

void fill_fibonacci(int *arr, int count) {
    if (count < 1 || arr == NULL) return;
    
    int *ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        if (i - 1 >= 0 && i - 2 >= 0) {
            long sum = (long)*(arr + i - 1) + (long)*(arr + i - 2);
            if (sum > INT_MAX || sum < INT_MIN) {
                *(arr + i) = 0;
            } else {
                *(arr + i) = (int)sum;
            }
        }
    }
}

int validate_integer(const char *str, int *value) {
    if (str == NULL || value == NULL) return 0;
    
    char *endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0' && *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *value = (int)val;
    return 1;
}

int main(void) {
    double coefficients[5] = {2.0, -3.0, 1.5, -0.5, 0.1};
    int degree = 4;
    
    printf("Polynomial coefficients: ");
    double *coeff_ptr = coefficients;
    for (int i = 0; i <= degree; i++) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    printf("Enter x value for polynomial evaluation: ");
    char input[100];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double x;
    char *endptr;
    x = strtod(input, &endptr);
    if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Invalid floating point value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x);
    printf("P(%.2f) = %.6f\n", x, result);
    
    int fibonacci_count;
    printf("Enter number of Fibonacci numbers to generate (1-%d): ", ARRAY_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_integer(input, &fibonacci_count)) {
        fprintf(stderr, "Invalid integer\n");
        return 1;
    }
    
    if (fibonacci_count < 1 || fibonacci_count > ARRAY_SIZE) {
        fprintf(stderr, "Count must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    int fib_array[ARRAY_SIZE];
    fill_fibonacci(fib_array, fibonacci_count);
    
    printf("First %d Fibonacci numbers: ", fibonacci_count);
    int *fib_ptr = fib_array;
    for (int i = 0; i < fibonacci_count; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    printf("Computing polynomial at Fibonacci indices:\n");
    for (int i = 0; i < fibonacci_count; i++) {
        double poly_val = compute_polynomial(coefficients, degree, (double)fib_array[i]);
        printf("P(%d) = %.6f\n", fib_array[i], poly_val);
    }
    
    return 0;
}