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
#define PI 3.14159265358979323846

double newton_sqrt(double n) {
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
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
    return x;
}

double compute_circle_area(double radius) {
    if (!VALIDATE_INPUT(radius) || radius < 0) return -1.0;
    return PI * SQUARE(radius);
}

double compute_sphere_volume(double radius) {
    if (!VALIDATE_INPUT(radius) || radius < 0) return -1.0;
    return (4.0 / 3.0) * PI * SQUARE(radius) * radius;
}

int main() {
    double input;
    char buffer[256];
    
    printf("Enter a positive number for radius calculations: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(buffer, "%lf", &input) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input)) {
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    double sqrt_result = newton_sqrt(input);
    if (sqrt_result < 0) {
        fprintf(stderr, "Cannot calculate square root of negative number\n");
        return 1;
    }
    
    double area = compute_circle_area(input);
    if (area < 0) {
        fprintf(stderr, "Invalid radius for area calculation\n");
        return 1;
    }
    
    double volume = compute_sphere_volume(input);
    if (volume < 0) {
        fprintf(stderr, "Invalid radius for volume calculation\n");
        return 1;
    }
    
    printf("Input value: %g\n", input);
    printf("Square root (Newton's method): %.10g\n", sqrt_result);
    printf("Standard library sqrt: %.10g\n", sqrt(input));
    printf("Circle area with radius %g: %.10g\n", input, area);
    printf("Sphere volume with radius %g: %.10g\n", input, volume);
    
    return 0;
}