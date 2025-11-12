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

double compute_hypotenuse(double a, double b) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || a < 0 || b < 0) return -1.0;
    return newton_sqrt(SQUARE(a) + SQUARE(b));
}

int main() {
    double num1, num2, radius;
    int choice;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root\n");
    printf("2. Compute circle area\n");
    printf("3. Compute hypotenuse\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter number: ");
            if (scanf("%lf", &num1) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(num1)) {
                printf("Input out of valid range.\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Cannot compute square root of negative number.\n");
                return 1;
            }
            printf("Square root: %.6f\n", newton_sqrt(num1));
            break;
            
        case 2:
            printf("Enter radius: ");
            if (scanf("%lf", &radius) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(radius)) {
                printf("Input out of valid range.\n");
                return 1;
            }
            if (radius < 0) {
                printf("Radius cannot be negative.\n");
                return 1;
            }
            printf("Circle area: %.6f\n", compute_circle_area(radius));
            break;
            
        case 3:
            printf("Enter side A: ");
            if (scanf("%lf", &num1) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            printf("Enter side B: ");
            if (scanf("%lf", &num2) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(num1) || !VALIDATE_INPUT(num2)) {
                printf("Input out of valid range.\n");
                return 1;
            }
            if (num1 < 0 || num2 < 0) {
                printf("Sides cannot be negative.\n");
                return 1;
            }
            printf("Hypotenuse: %.6f\n", compute_hypotenuse(num1, num2));
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}