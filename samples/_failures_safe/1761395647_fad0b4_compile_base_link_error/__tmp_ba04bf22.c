//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_root(double a, double b, double c) {
    double discriminant = SQUARE(b) - 4.0 * a * c;
    if (discriminant < -EPSILON) return NAN;
    if (ABS(discriminant) < EPSILON) return -b / (2.0 * a);
    return (-b + sqrt(discriminant)) / (2.0 * a);
}

double compute_integral(double (*f)(double), double start, double end, int steps) {
    if (steps <= 0 || steps > MAX_ITERATIONS) return NAN;
    double step_size = (end - start) / steps;
    double sum = 0.0;
    for (int i = 0; i < steps; i++) {
        double x1 = start + i * step_size;
        double x2 = x1 + step_size;
        sum += (f(x1) + f(x2)) * step_size / 2.0;
    }
    return sum;
}

double sample_function(double x) {
    return SQUARE(x) - 2.0 * x + 1.0;
}

int main() {
    double a, b, c;
    printf("Enter coefficients for quadratic equation (a b c): ");
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || !VALIDATE_INPUT(c)) {
        printf("Input values out of range.\n");
        return EXIT_FAILURE;
    }
    if (ABS(a) < EPSILON) {
        printf("Coefficient a cannot be zero.\n");
        return EXIT_FAILURE;
    }

    double root = compute_root(a, b, c);
    if (isnan(root)) {
        printf("No real roots found.\n");
    } else {
        printf("One real root: %.6f\n", root);
    }

    double start, end;
    int steps;
    printf("Enter integration range and steps (start end steps): ");
    if (scanf("%lf %lf %d", &start, &end, &steps) != 3) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    if (!VALIDATE_INPUT(start) || !VALIDATE_INPUT(end)) {
        printf("Input values out of range.\n");
        return EXIT_FAILURE;
    }
    if (steps <= 0 || steps > MAX_ITERATIONS) {
        printf("Invalid number of steps.\n");
        return EXIT_FAILURE;
    }

    double integral = compute_integral(sample_function, start, end, steps);
    if (isnan(integral)) {
        printf("Integration failed.\n");
    } else {
        printf("Approximate integral: %.6f\n", integral);
    }

    return EXIT_SUCCESS;
}