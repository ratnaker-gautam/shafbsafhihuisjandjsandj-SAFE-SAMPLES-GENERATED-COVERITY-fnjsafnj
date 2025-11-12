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
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double inputs[] = {2.0, 3.0, 0.0, -4.0, 16.0};
    int num_inputs = sizeof(inputs) / sizeof(inputs[0]);
    
    for (int i = 0; i < num_functions; i++) {
        for (int j = 0; j < num_inputs; j++) {
            compute_and_print(functions[i], names[i], inputs[j]);
        }
        printf("\n");
    }
    
    return 0;
}