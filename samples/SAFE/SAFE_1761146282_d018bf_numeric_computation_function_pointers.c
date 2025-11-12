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
    if (fabs(x) < 1e-12) {
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

void compute_and_print(numeric_func f, double value, const char* name) {
    double result = f(value);
    if (isnan(result)) {
        printf("%s(%.2f) = undefined\n", name, value);
    } else {
        printf("%s(%.2f) = %.4f\n", name, value, result);
    }
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double input_value;
    printf("Enter a numeric value: ");
    
    if (scanf("%lf", &input_value) != 1) {
        printf("Error: Invalid input. Please enter a numeric value.\n");
        return 1;
    }
    
    if (!isfinite(input_value)) {
        printf("Error: Input value must be finite.\n");
        return 1;
    }
    
    printf("Computing functions for value: %.2f\n", input_value);
    printf("--------------------------------\n");
    
    for (int i = 0; i < num_functions; i++) {
        compute_and_print(functions[i], input_value, names[i]);
    }
    
    printf("--------------------------------\n");
    
    return 0;
}