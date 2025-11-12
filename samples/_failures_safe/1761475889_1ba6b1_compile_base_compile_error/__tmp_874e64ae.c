//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define ARRAY_SIZE 10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_EVEN(x) ((x) % 2 == 0)
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define M_PI 3.14159265358979323846

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
    }
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        double term = 1.0 / (2 * i + 1);
        if (IS_EVEN(i)) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

double newton_sqrt(double n) {
    if (n < 0.0) {
        return 0.0;
    }
    if (n == 0.0) {
        return 0.0;
    }
    double x = n;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (x == 0.0) {
            return 0.0;
        }
        double next_x = 0.5 * (x + n / x);
        if (ABS(next_x - x) < EPSILON) {
            return next_x;
        }
        x = next_x;
    }
    return x;
}

int main(void) {
    int user_iterations;
    printf("Enter number of iterations for pi calculation (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &user_iterations) != 1) {
        printf("Invalid input. Using default iterations.\n");
        user_iterations = MAX_ITERATIONS;
    }
    user_iterations = CLAMP(user_iterations, 1, MAX_ITERATIONS);
    
    double pi_approx = compute_pi_approximation(user_iterations);
    printf("Pi approximation after %d iterations: %.10f\n", user_iterations, pi_approx);
    printf("Difference from math.h pi: %.10f\n", ABS(pi_approx - M_PI));
    
    double numbers[ARRAY_SIZE];
    printf("Enter %d positive numbers for sqrt calculation:\n", ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (scanf("%lf", &numbers[i]) != 1) {
            numbers[i] = 1.0;
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        numbers[i] = CLAMP(numbers[i], 0.0, 1e6);
    }
    
    printf("\nSquare root calculations:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double sqrt_val = newton_sqrt(numbers[i]);
        double math_sqrt = sqrt(numbers[i]);
        printf("Number: %8.3f, Newton: %8.6f, math.h: %8.6f, Diff: %10.8f\n",
               numbers[i], sqrt_val, math_sqrt, ABS(sqrt_val - math_sqrt));
    }
    
    int test_values[] = {-5, 0, 7, 12, 25};
    printf("\nEven/odd test:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d is %s\n", test_values[i], IS_EVEN(test_values[i]) ? "even" : "odd");
    }
    
    return 0;
}