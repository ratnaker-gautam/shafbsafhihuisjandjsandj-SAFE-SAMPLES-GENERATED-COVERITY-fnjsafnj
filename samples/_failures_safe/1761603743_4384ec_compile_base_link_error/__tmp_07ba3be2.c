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
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_sqrt_newton(double n) {
    if (n < 0) return NAN;
    if (n == 0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }
    
    double result = 1.0;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

int main(void) {
    double input_num;
    int exponent;
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
    
    if (!VALIDATE_INPUT(input_num)) {
        fprintf(stderr, "Number out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer exponent (-100 to 100): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(buffer, "%d", &exponent) != 1) {
        fprintf(stderr, "Invalid exponent format\n");
        return EXIT_FAILURE;
    }
    
    if (exponent < -100 || exponent > 100) {
        fprintf(stderr, "Exponent out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    double power_result = compute_power(input_num, exponent);
    
    if (isnan(sqrt_result)) {
        printf("Square root of %.2f is undefined (negative input)\n", input_num);
    } else {
        printf("Square root of %.2f: %.10f\n", input_num, sqrt_result);
        printf("Library sqrt result: %.10f\n", sqrt(input_num));
    }
    
    printf("%.2f^%d: %.10f\n", input_num, exponent, power_result);
    printf("Library pow result: %.10f\n", pow(input_num, exponent));
    
    return EXIT_SUCCESS;
}