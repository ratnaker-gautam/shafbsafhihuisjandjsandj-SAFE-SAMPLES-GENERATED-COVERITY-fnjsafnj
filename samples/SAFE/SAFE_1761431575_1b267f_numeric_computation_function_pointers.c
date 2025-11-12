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
    printf("%s:\n", name);
    
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        double result = func(x);
        
        if (isnan(result)) {
            printf("  f(%.3f) = undefined\n", x);
        } else if (isinf(result)) {
            printf("  f(%.3f) = infinity\n", x);
        } else {
            printf("  f(%.3f) = %.3f\n", x, result);
        }
    }
    printf("\n");
}

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min_val, max_val);
    }
}

double read_double(const char* prompt) {
    double value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0.0;
        }
        
        if (sscanf(buffer, "%lf", &value) == 1) {
            return value;
        }
        printf("Invalid input. Please enter a valid number.\n");
    }
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"Square", "Cube", "Reciprocal", "Square Root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    printf("Numeric Function Computation\n");
    printf("============================\n\n");
    
    double start = read_double("Enter start value: ");
    double end = read_double("Enter end value: ");
    
    if (start > end) {
        double temp = start;
        start = end;
        end = temp;
        printf("Swapped start and end values.\n");
    }
    
    int steps = read_int("Enter number of steps (1-100): ", 1, 100);
    
    printf("\nComputing functions from %.3f to %.3f in %d steps:\n\n", start, end, steps);
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return 0;
}