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
    if (fabs(x) < 1e-15) {
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
    if (isnan(step_size) || isinf(step_size)) {
        return;
    }
    
    printf("Applying %s from %.2f to %.2f in %d steps:\n", name, start, end, steps);
    
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        if (isnan(x) || isinf(x)) {
            continue;
        }
        
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

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double ranges[][2] = {{-2.0, 2.0}, {0.0, 3.0}, {-1.0, 1.0}, {0.0, 4.0}};
    int steps[] = {8, 6, 10, 8};
    
    for (int i = 0; i < num_functions; i++) {
        if (i >= sizeof(ranges) / sizeof(ranges[0]) || i >= sizeof(steps) / sizeof(steps[0])) {
            break;
        }
        
        double start = ranges[i][0];
        double end = ranges[i][1];
        int step_count = steps[i];
        
        if (step_count <= 0 || start > end) {
            continue;
        }
        
        apply_function(functions[i], names[i], start, end, step_count);
    }
    
    return 0;
}