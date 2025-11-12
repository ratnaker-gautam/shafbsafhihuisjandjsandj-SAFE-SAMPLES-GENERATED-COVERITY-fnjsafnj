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
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (fabs(term) >= epsilon && n < MAX_ITERATIONS) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (n >= MAX_ITERATIONS) {
            break;
        }
    }
    
    return result;
}

double compute_cosine_series(double x, double epsilon) {
    double result = 1.0;
    double term = 1.0;
    int n = 1;
    
    while (fabs(term) >= epsilon && n < MAX_ITERATIONS) {
        term = -term * x * x / ((2 * n - 1) * (2 * n));
        result += term;
        n++;
        
        if (n >= MAX_ITERATIONS) {
            break;
        }
    }
    
    return result;
}

int main(void) {
    double x, epsilon;
    char input_buffer[100];
    
    printf("Enter x value (-100 to 100): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    printf("Enter precision epsilon (0 < epsilon < 1): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%lf", &epsilon) != 1) {
        fprintf(stderr, "Invalid epsilon value\n");
        return 1;
    }
    
    if (!validate_input(x, epsilon)) {
        fprintf(stderr, "Invalid input parameters\n");
        return 1;
    }
    
    double sine_approx = compute_sine_series(x, epsilon);
    double cosine_approx = compute_cosine_series(x, epsilon);
    double library_sine = sin(x);
    double library_cosine = cos(x);
    
    printf("Sine approximation: %.10f\n", sine_approx);
    printf("Library sine: %.10f\n", library_sine);
    printf("Absolute error: %.10f\n", fabs(sine_approx - library_sine));
    
    printf("Cosine approximation: %.10f\n", cosine_approx);
    printf("Library cosine: %.10f\n", library_cosine);
    printf("Absolute error: %.10f\n", fabs(cosine_approx - library_cosine));
    
    return 0;
}