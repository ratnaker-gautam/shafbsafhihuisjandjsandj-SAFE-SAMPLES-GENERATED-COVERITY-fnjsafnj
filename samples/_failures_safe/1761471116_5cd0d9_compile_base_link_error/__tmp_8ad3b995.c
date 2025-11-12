//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int validate_input(double x, double epsilon) {
    if (epsilon <= 0.0 || epsilon >= 1.0) {
        return 0;
    }
    if (x < -100.0 || x > 100.0) {
        return 0;
    }
    return 1;
}

double compute_sine_series(double x, double epsilon) {
    double term = x;
    double sum = term;
    int n = 1;
    
    while (n < MAX_ITERATIONS) {
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        if (fabs(term) < epsilon) {
            break;
        }
        sum += term;
        n++;
    }
    return sum;
}

double compute_cosine_series(double x, double epsilon) {
    double term = 1.0;
    double sum = term;
    int n = 1;
    
    while (n < MAX_ITERATIONS) {
        term = -term * x * x / ((2 * n - 1) * (2 * n));
        if (fabs(term) < epsilon) {
            break;
        }
        sum += term;
        n++;
    }
    return sum;
}

int main(void) {
    double x, epsilon;
    char input_buffer[256];
    
    printf("Enter angle in radians (-100 to 100): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Enter precision epsilon (0 < epsilon < 1): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%lf", &epsilon) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!validate_input(x, epsilon)) {
        fprintf(stderr, "Invalid input values\n");
        return 1;
    }
    
    double sine_result = compute_sine_series(x, epsilon);
    double cosine_result = compute_cosine_series(x, epsilon);
    double library_sine = sin(x);
    double library_cosine = cos(x);
    
    printf("Taylor series sine: %.10f\n", sine_result);
    printf("Library sine: %.10f\n", library_sine);
    printf("Difference: %.10f\n", fabs(sine_result - library_sine));
    
    printf("Taylor series cosine: %.10f\n", cosine_result);
    printf("Library cosine: %.10f\n", library_cosine);
    printf("Difference: %.10f\n", fabs(cosine_result - library_cosine));
    
    return 0;
}