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

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    int valid = 0;
    
    while (!valid) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            exit(1);
        }
        
        char* endptr;
        value = strtod(buffer, &endptr);
        
        if (endptr != buffer && *endptr == '\n') {
            valid = 1;
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }
    
    return value;
}

void apply_function(numeric_func func, const char* name, double x) {
    double result = func(x);
    
    printf("Function: %s\n", name);
    printf("Input: %.6f\n", x);
    
    if (isnan(result)) {
        printf("Result: undefined\n");
    } else if (isinf(result)) {
        printf("Result: infinite\n");
    } else {
        printf("Result: %.6f\n", result);
    }
    printf("---\n");
}

int main() {
    double input_value;
    
    printf("Numeric Function Computation Demo\n");
    printf("=================================\n\n");
    
    input_value = safe_input("Enter a numeric value: ");
    
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    size_t num_functions = sizeof(functions) / sizeof(functions[0]);
    
    printf("\nApplying numeric functions:\n\n");
    
    for (size_t i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], input_value);
    }
    
    printf("Computation complete.\n");
    
    return 0;
}