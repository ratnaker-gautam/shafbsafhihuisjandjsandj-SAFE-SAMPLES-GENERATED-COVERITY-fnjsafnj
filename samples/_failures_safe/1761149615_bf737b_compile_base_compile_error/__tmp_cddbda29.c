//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define PI_CONST 3.14159265358979323846

double compute_pi_approximation(int terms) {
    if (terms <= 0 || terms > MAX_ITERATIONS) {
        return -1.0;
    }
    double sum = 0.0;
    for (int i = 0; i < terms; i++) {
        double term = 1.0 / (2 * i + 1);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

double compute_sqrt_newton(double number) {
    if (number < 0.0) {
        return -1.0;
    }
    if (number == 0.0) {
        return 0.0;
    }
    double guess = CLAMP(number, 1.0, 1000000.0);
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double new_guess = 0.5 * (guess + number / guess);
        if (ABS(new_guess - guess) < EPSILON) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

int main(void) {
    int choice;
    printf("Numeric Computation Menu:\n");
    printf("1. Approximate Pi using series\n");
    printf("2. Compute square root using Newton's method\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice == 1) {
        int terms;
        printf("Enter number of terms (1-%d): ", MAX_ITERATIONS);
        if (scanf("%d", &terms) != 1) {
            printf("Invalid input.\n");
            return 1;
        }
        double result = compute_pi_approximation(terms);
        if (result < 0.0) {
            printf("Invalid number of terms.\n");
            return 1;
        }
        printf("Pi approximation with %d terms: %.10f\n", terms, result);
        printf("Difference from math.h pi: %.10f\n", ABS(result - PI_CONST));
    } else if (choice == 2) {
        double number;
        printf("Enter a non-negative number: ");
        if (scanf("%lf", &number) != 1) {
            printf("Invalid input.\n");
            return 1;
        }
        double result = compute_sqrt_newton(number);
        if (result < 0.0) {
            printf("Cannot compute square root of negative number.\n");
            return 1;
        }
        double math_sqrt = sqrt(number);
        printf("Square root of %.6f: %.10f\n", number, result);
        printf("Difference from math.h sqrt: %.10f\n", ABS(result - math_sqrt));
    } else {
        printf("Invalid choice.\n");
        return 1;
    }
    
    return 0;
}