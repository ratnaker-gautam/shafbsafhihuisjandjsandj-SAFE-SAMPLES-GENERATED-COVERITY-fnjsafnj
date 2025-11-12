//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_pow(double base, double exponent) {
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) {
        return NAN;
    }
    if (base == 0.0 && exponent < 0.0) {
        return INFINITY;
    }
    return pow(base, exponent);
}

double compute_series(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return NAN;
    }
    double result = 0.0;
    double term = 1.0;
    for (int i = 1; i <= n_terms; i++) {
        term *= x / (double)i;
        result += term;
        if (!isfinite(result)) {
            return NAN;
        }
    }
    return result;
}

double validate_input_double(const char* prompt, double min_val, double max_val) {
    char buffer[256];
    double value;
    int valid;
    
    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return NAN;
        }
        valid = sscanf(buffer, "%lf", &value);
        if (valid != 1 || value < min_val || value > max_val || !isfinite(value)) {
            printf("Invalid input. Please enter a number between %g and %g.\n", min_val, max_val);
            valid = 0;
        }
    } while (!valid);
    
    return value;
}

int validate_input_int(const char* prompt, int min_val, int max_val) {
    char buffer[256];
    int value;
    int valid;
    
    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        valid = sscanf(buffer, "%d", &value);
        if (valid != 1 || value < min_val || value > max_val) {
            printf("Invalid input. Please enter an integer between %d and %d.\n", min_val, max_val);
            valid = 0;
        }
    } while (!valid);
    
    return value;
}

int main(void) {
    printf("Numerical Computation Demo\n");
    printf("==========================\n\n");
    
    double x = validate_input_double("Enter value for x (1e-10 to 1e6): ", MIN_INPUT, MAX_INPUT);
    if (isnan(x)) {
        fprintf(stderr, "Input error.\n");
        return EXIT_FAILURE;
    }
    
    int terms = validate_input_int("Enter number of terms (1 to 1000): ", 1, MAX_ITERATIONS);
    if (terms == -1) {
        fprintf(stderr, "Input error.\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series(x, terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Series computation failed.\n");
        return EXIT_FAILURE;
    }
    
    double power_result = safe_pow(x, 2.5);
    if (isnan(power_result) || !isfinite(power_result)) {
        fprintf(stderr, "Power computation failed.\n");
        return EXIT_FAILURE;
    }
    
    double final_result = series_result * log1p(fabs(power_result));
    if (!isfinite(final_result)) {
        fprintf(stderr, "Final computation failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("\nResults:\n");
    printf("Series approximation (e^%g): %.10g\n", x, series_result);
    printf("Power result (%g^2.5): %.10g\n", x, power_result);
    printf("Final computation: %.10g\n", final_result);
    
    return EXIT_SUCCESS;
}