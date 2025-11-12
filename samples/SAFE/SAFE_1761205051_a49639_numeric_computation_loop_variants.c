//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Numerical overflow detected\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Numerical overflow in sum\n");
            exit(EXIT_FAILURE);
        }
    }
    return sum;
}

double compute_approximation() {
    double x = 1.0;
    int i = 0;
    while (i < MAX_ITERATIONS) {
        double fx = x * x - 2.0;
        double fpx = 2.0 * x;
        if (fabs(fpx) < TOLERANCE) {
            fprintf(stderr, "Derivative too small\n");
            exit(EXIT_FAILURE);
        }
        double x_new = x - fx / fpx;
        if (isinf(x_new) || isnan(x_new)) {
            fprintf(stderr, "Numerical overflow in Newton's method\n");
            exit(EXIT_FAILURE);
        }
        if (fabs(x_new - x) < TOLERANCE) {
            return x_new;
        }
        x = x_new;
        i++;
    }
    fprintf(stderr, "Newton's method did not converge\n");
    exit(EXIT_FAILURE);
}

int main() {
    int n;
    printf("Enter number of terms for series sum (1-10000): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    if (n < 1 || n > 10000) {
        fprintf(stderr, "Input out of range\n");
        return EXIT_FAILURE;
    }

    double series_sum = compute_series_sum(n);
    printf("Series sum for %d terms: %.10f\n", n, series_sum);

    double sqrt2_approx = compute_approximation();
    printf("Approximation of sqrt(2): %.10f\n", sqrt2_approx);

    double error = fabs(sqrt2_approx - sqrt(2.0));
    printf("Approximation error: %.2e\n", error);

    double product = 1.0;
    int count = 0;
    do {
        double factor = 1.0 + (1.0 / (count + 1.0));
        if (isinf(factor) || isnan(factor)) {
            fprintf(stderr, "Numerical overflow in product\n");
            return EXIT_FAILURE;
        }
        product *= factor;
        if (isinf(product) || isnan(product)) {
            fprintf(stderr, "Numerical overflow in product\n");
            return EXIT_FAILURE;
        }
        count++;
    } while (count < 10 && product < 100.0);

    printf("Final product after %d iterations: %.6f\n", count, product);

    return EXIT_SUCCESS;
}