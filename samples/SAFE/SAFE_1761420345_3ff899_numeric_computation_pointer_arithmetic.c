//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double x, const double *coeffs, int degree) {
    if (degree < 0 || coeffs == NULL) return 0.0;
    
    double result = 0.0;
    const double *ptr = coeffs + degree;
    
    while (ptr >= coeffs) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

int read_coefficients(double *buffer, int max_degree) {
    if (buffer == NULL || max_degree <= 0) return -1;
    
    char input[1024];
    printf("Enter polynomial coefficients from highest to lowest degree (space separated): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *token = strtok(input, " \t\n");
    int count = 0;
    
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) return -1;
        
        *(buffer + count) = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count > 0 ? count - 1 : -1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    int degree;
    
    degree = read_coefficients(coefficients, ARRAY_SIZE - 1);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value for polynomial evaluation: ");
    double x;
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Computation resulted in non-finite value\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x, result);
    
    printf("Coefficient addresses and values:\n");
    const double *start = coefficients;
    const double *end = coefficients + degree;
    
    while (start <= end) {
        int index = (int)(start - coefficients);
        printf("Coefficient %d [%p]: %.6f\n", index, (void*)start, *start);
        start++;
    }
    
    return 0;
}