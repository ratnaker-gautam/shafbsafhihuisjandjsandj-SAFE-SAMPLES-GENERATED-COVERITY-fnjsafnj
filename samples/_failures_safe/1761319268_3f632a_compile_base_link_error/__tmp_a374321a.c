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

void compute_and_print(numeric_func f, double value, const char* name) {
    double result = f(value);
    if (isnan(result)) {
        printf("%s(%.2f) = undefined\n", name, value);
    } else {
        printf("%s(%.2f) = %.4f\n", name, value, result);
    }
}

int main() {
    double input;
    printf("Enter a numeric value: ");
    
    if (scanf("%lf", &input) != 1) {
        printf("Error: Invalid input. Please enter a number.\n");
        return 1;
    }
    
    printf("\nComputing various functions for value %.2f:\n", input);
    printf("==========================================\n");
    
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    for (int i = 0; i < num_functions; i++) {
        compute_and_print(functions[i], input, names[i]);
    }
    
    printf("\nComputing function composition:\n");
    printf("================================\n");
    
    double temp = square(input);
    if (!isnan(temp)) {
        double final_result = square_root(temp);
        if (isnan(final_result)) {
            printf("square_root(square(%.2f)) = undefined\n", input);
        } else {
            printf("square_root(square(%.2f)) = %.4f\n", input, final_result);
        }
    } else {
        printf("square_root(square(%.2f)) = undefined\n", input);
    }
    
    return 0;
}