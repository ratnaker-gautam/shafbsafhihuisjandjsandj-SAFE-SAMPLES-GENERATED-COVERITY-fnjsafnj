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

void apply_function(numeric_func func, const char* name, double start, double end, int steps) {
    if (steps <= 0 || start > end || func == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    if (step_size <= 0) {
        return;
    }
    
    printf("Applying %s from %.2f to %.2f in %d steps:\n", name, start, end, steps);
    
    for (int i = 0; i <= steps; i++) {
        double x = start + (i * step_size);
        double result = func(x);
        
        if (isnan(result)) {
            printf("  f(%.2f) = undefined\n", x);
        } else if (isinf(result)) {
            printf("  f(%.2f) = infinity\n", x);
        } else {
            printf("  f(%.2f) = %.4f\n", x, result);
        }
    }
    printf("\n");
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

double get_valid_double(const char* prompt) {
    char buffer[100];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%lf", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        return value;
    }
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    printf("Numeric Function Computation Demo\n");
    printf("=================================\n\n");
    
    double start = get_valid_double("Enter start value: ");
    double end = get_valid_double("Enter end value: ");
    
    if (start > end) {
        double temp = start;
        start = end;
        end = temp;
        printf("Swapped start and end values.\n");
    }
    
    int steps = get_valid_int("Enter number of steps (1-100): ", 1, 100);
    
    printf("\n");
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return 0;
}