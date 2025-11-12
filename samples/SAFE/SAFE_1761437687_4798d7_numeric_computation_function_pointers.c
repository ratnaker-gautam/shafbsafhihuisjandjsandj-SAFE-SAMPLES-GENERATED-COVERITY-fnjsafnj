//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double (*numeric_func)(double);

double square(double x) {
    return x * x;
}

double cube(double x) {
    return x * x * x;
}

double reciprocal(double x) {
    if (fabs(x) < 1e-10) {
        return NAN;
    }
    return 1.0 / x;
}

double square_root(double x) {
    if (x < 0) {
        return NAN;
    }
    return sqrt(x);
}

void apply_function(numeric_func f, const char* name, double start, double end, double step) {
    if (step <= 0 || start > end) {
        printf("Invalid range parameters.\n");
        return;
    }
    
    printf("Applying %s from %.2f to %.2f (step %.2f):\n", name, start, end, step);
    
    for (double x = start; x <= end + 1e-9; x += step) {
        double result = f(x);
        if (isnan(result)) {
            printf("  f(%.2f) = undefined\n", x);
        } else {
            printf("  f(%.2f) = %.4f\n", x, result);
        }
    }
    printf("\n");
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start, end, step;
    
    printf("Enter start value: ");
    if (scanf("%lf", &start) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    printf("Enter end value: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    printf("Enter step size: ");
    if (scanf("%lf", &step) != 1 || step <= 0) {
        printf("Invalid step size.\n");
        return 1;
    }
    
    if (start > end) {
        printf("Start value cannot be greater than end value.\n");
        return 1;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, step);
    }
    
    return 0;
}