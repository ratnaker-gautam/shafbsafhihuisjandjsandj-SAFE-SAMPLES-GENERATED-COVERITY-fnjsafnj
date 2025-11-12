//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_EVEN(n) ((n) % 2 == 0)

double compute_series_sum(int terms) {
    if (terms <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        double term = IS_EVEN(i) ? -1.0 : 1.0;
        term /= (2.0 * i - 1.0);
        sum += term;
    }
    return 4.0 * sum;
}

double compute_pi_bisection() {
    double left = 3.0;
    double right = 4.0;
    double mid;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS && (right - left) > EPSILON) {
        mid = (left + right) / 2.0;
        double f_mid = sin(mid);
        
        if (ABS(f_mid) < EPSILON) {
            break;
        }
        
        if (f_mid > 0.0) {
            left = mid;
        } else {
            right = mid;
        }
        iterations++;
    }
    return mid;
}

int validate_input(const char* input, int* value) {
    if (input == NULL || value == NULL) return 0;
    
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (temp <= 0 || temp > INT_MAX) return 0;
    
    *value = (int)temp;
    return 1;
}

int main() {
    char buffer[256];
    int terms;
    
    printf("Enter number of terms for series computation (1-1000): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input(buffer, &terms)) {
        fprintf(stderr, "Invalid input. Please enter a positive integer.\n");
        return 1;
    }
    
    if (terms > 1000) {
        fprintf(stderr, "Too many terms. Using maximum of 1000.\n");
        terms = 1000;
    }
    
    double series_pi = compute_series_sum(terms);
    double bisection_pi = compute_pi_bisection();
    double difference = ABS(series_pi - bisection_pi);
    
    printf("Series approximation (%d terms): %.10f\n", terms, series_pi);
    printf("Bisection method approximation: %.10f\n", bisection_pi);
    printf("Difference: %.10f\n", difference);
    
    return 0;
}