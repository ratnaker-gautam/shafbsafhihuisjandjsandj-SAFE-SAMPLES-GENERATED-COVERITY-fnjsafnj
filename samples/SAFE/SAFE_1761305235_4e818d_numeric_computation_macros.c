//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_sqrt_newton(double n) {
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    double x = n;
    double prev;
    int iter = 0;
    do {
        prev = x;
        if (x == 0.0) return -1.0;
        x = 0.5 * (x + n / x);
        iter++;
        if (iter >= MAX_ITERATIONS) break;
    } while (ABS(x - prev) > EPSILON);
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        if (base == 0.0) return -1.0;
        base = 1.0 / base;
        exponent = -exponent;
    }
    double result = 1.0;
    while (exponent > 0) {
        if (exponent & 1) {
            if (result > DBL_MAX / base) return -1.0;
            result *= base;
        }
        if (exponent > 1) {
            if (base > DBL_MAX / base) return -1.0;
            base *= base;
        }
        exponent >>= 1;
    }
    return result;
}

int main() {
    double input_num;
    int input_exp;
    char buffer[256];
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    if (sscanf(buffer, "%lf", &input_num) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return EXIT_FAILURE;
    }
    if (!IS_VALID(input_num)) {
        fprintf(stderr, "Number out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer exponent (-100 to 100): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    if (sscanf(buffer, "%d", &input_exp) != 1) {
        fprintf(stderr, "Invalid exponent format\n");
        return EXIT_FAILURE;
    }
    if (input_exp < -100 || input_exp > 100) {
        fprintf(stderr, "Exponent out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    if (sqrt_result < 0) {
        printf("Cannot compute square root of negative number\n");
    } else {
        printf("Square root: %.10f\n", sqrt_result);
    }
    
    double power_result = compute_power(input_num, input_exp);
    if (power_result < 0) {
        printf("Power computation failed (overflow or division by zero)\n");
    } else {
        printf("Power result: %.10f\n", power_result);
    }
    
    return EXIT_SUCCESS;
}