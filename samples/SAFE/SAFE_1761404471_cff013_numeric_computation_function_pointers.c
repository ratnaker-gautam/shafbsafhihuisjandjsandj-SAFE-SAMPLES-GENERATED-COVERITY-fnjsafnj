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

void compute_and_display(numeric_func func, double value, const char* name) {
    double result = func(value);
    if (isnan(result)) {
        printf("%s(%.2f) = undefined\n", name, value);
    } else {
        printf("%s(%.2f) = %.4f\n", name, value, result);
    }
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double input_value;
    printf("Enter a numeric value: ");
    
    if (scanf("%lf", &input_value) != 1) {
        printf("Error: Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    if (!isfinite(input_value)) {
        printf("Error: Input value must be finite.\n");
        return 1;
    }
    
    printf("\nComputing various functions for value: %.2f\n\n", input_value);
    
    for (int i = 0; i < num_functions; i++) {
        compute_and_display(functions[i], input_value, names[i]);
    }
    
    printf("\nComputing function composition:\n");
    double temp = functions[0](input_value);
    double final_result = functions[3](temp);
    
    if (isnan(final_result)) {
        printf("square_root(square(%.2f)) = undefined\n", input_value);
    } else {
        printf("square_root(square(%.2f)) = %.4f\n", input_value, final_result);
    }
    
    return 0;
}