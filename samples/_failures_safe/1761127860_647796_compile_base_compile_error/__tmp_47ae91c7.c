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

double compute_compound_interest(double principal, double rate, int years) {
    if (!VALIDATE_INPUT(principal) || !VALIDATE_INPUT(rate) || years < 0 || years > 100) return -1.0;
    if (principal < 0 || rate < 0) return -1.0;
    
    double result = principal * pow(1.0 + rate / 100.0, years);
    if (result < 0 || !VALIDATE_INPUT(result)) return -1.0;
    return result;
}

int main() {
    double input1, input2;
    int input3;
    
    printf("Enter a number to compute square root: ");
    if (scanf("%lf", &input1) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    double sqrt_result = newton_sqrt(input1);
    if (sqrt_result < 0) {
        printf("Cannot compute square root of negative number\n");
    } else {
        printf("Square root: %.6f\n", sqrt_result);
    }
    
    printf("Enter circle radius: ");
    if (scanf("%lf", &input1) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    double area = compute_circle_area(input1);
    if (area < 0) {
        printf("Invalid radius\n");
    } else {
        printf("Circle area: %.6f\n", area);
    }
    
    printf("Enter principal, rate%%, years: ");
    if (scanf("%lf %lf %d", &input1, &input2, &input3) != 3) {
        printf("Invalid input\n");
        return 1;
    }
    
    double interest = compute_compound_interest(input1, input2, input3);
    if (interest < 0) {
        printf("Invalid inputs for compound interest\n");
    } else {
        printf("Compound interest: %.2f\n", interest);
    }
    
    return 0;
}