//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

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

double apply_function(numeric_func f, double x) {
    if (f == NULL) {
        return NAN;
    }
    return f(x);
}

int main(void) {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    char* names[] = {"square", "cube", "reciprocal", "square root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double input;
    int choice;
    
    printf("Enter a number: ");
    if (scanf("%lf", &input) != 1) {
        printf("Invalid input. Must be a number.\n");
        return 1;
    }
    
    printf("Choose a function:\n");
    for (int i = 0; i < num_functions; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Enter choice (1-%d): ", num_functions);
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_functions) {
        printf("Choice out of range.\n");
        return 1;
    }
    
    numeric_func selected_func = functions[choice - 1];
    double result = apply_function(selected_func, input);
    
    if (isnan(result)) {
        printf("Error: Invalid operation for input %g\n", input);
        return 1;
    }
    
    printf("Result of %s(%g) = %g\n", names[choice - 1], input, result);
    
    return 0;
}