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

void compute_and_print(numeric_func f, const char* name, double x) {
    double result = f(x);
    if (isnan(result)) {
        printf("%s(%.2f) = undefined\n", name, x);
    } else {
        printf("%s(%.2f) = %.4f\n", name, x, result);
    }
}

int main() {
    double input;
    printf("Enter a numeric value: ");
    
    if (scanf("%lf", &input) != 1) {
        printf("Error: Invalid input. Please enter a number.\n");
        return 1;
    }
    
    if (!isfinite(input)) {
        printf("Error: Input must be a finite number.\n");
        return 1;
    }
    
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    printf("Computing various functions for value: %.2f\n", input);
    for (int i = 0; i < num_functions; i++) {
        compute_and_print(functions[i], names[i], input);
    }
    
    return 0;
}