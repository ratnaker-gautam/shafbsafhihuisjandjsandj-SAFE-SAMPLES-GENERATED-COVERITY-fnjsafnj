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
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double input;
    printf("Enter a number: ");
    
    if (scanf("%lf", &input) != 1) {
        printf("Error: Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    if (!isfinite(input)) {
        printf("Error: Input must be a finite number.\n");
        return 1;
    }
    
    printf("Computing functions for input: %.2f\n", input);
    
    for (int i = 0; i < num_functions; i++) {
        compute_and_print(functions[i], names[i], input);
    }
    
    printf("Computing function compositions:\n");
    for (int i = 0; i < num_functions; i++) {
        for (int j = 0; j < num_functions; j++) {
            double first_result = functions[i](input);
            if (!isnan(first_result)) {
                double second_result = functions[j](first_result);
                if (!isnan(second_result)) {
                    printf("%s(%s(%.2f)) = %.4f\n", names[j], names[i], input, second_result);
                }
            }
        }
    }
    
    return 0;
}