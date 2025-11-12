//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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

void apply_function(numeric_func func, double start, double end, double step) {
    if (step <= 0 || start > end) {
        printf("Invalid range parameters\n");
        return;
    }
    
    printf("x\t\tf(x)\n");
    printf("----------------------\n");
    
    double x = start;
    while (x <= end) {
        double result = func(x);
        if (isnan(result)) {
            printf("%.2f\t\tundefined\n", x);
        } else {
            printf("%.2f\t\t%.4f\n", x, result);
        }
        
        if (x > DBL_MAX - step) {
            break;
        }
        x += step;
    }
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    char* names[] = {"square", "cube", "reciprocal", "square root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    printf("Available functions:\n");
    for (int i = 0; i < num_functions; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select function (1-%d): ", num_functions);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_functions) {
        printf("Invalid choice\n");
        return 1;
    }
    
    double start, end, step;
    printf("Enter start value: ");
    if (scanf("%lf", &start) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter end value: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter step size: ");
    if (scanf("%lf", &step) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (step <= 0) {
        printf("Step size must be positive\n");
        return 1;
    }
    
    if (start > end) {
        printf("Start must be less than or equal to end\n");
        return 1;
    }
    
    printf("\nFunction: %s\n", names[choice - 1]);
    printf("Range: [%.2f, %.2f], Step: %.2f\n\n", start, end, step);
    
    apply_function(functions[choice - 1], start, end, step);
    
    return 0;
}