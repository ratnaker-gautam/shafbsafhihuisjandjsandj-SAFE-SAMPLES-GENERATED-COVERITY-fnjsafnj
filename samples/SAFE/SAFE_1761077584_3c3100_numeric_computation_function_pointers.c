//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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

int validate_input(const char *input, double *result) {
    char *endptr;
    double val = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    if (val == HUGE_VAL || val == -HUGE_VAL) {
        return 0;
    }
    *result = val;
    return 1;
}

void print_result(double input, double result) {
    if (isnan(result)) {
        printf("Result: undefined\n");
    } else if (isinf(result)) {
        printf("Result: infinite\n");
    } else {
        printf("Result: %.6f\n", result);
    }
}

int main(void) {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char *names[] = {"square", "cube", "reciprocal", "square root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    char input_buffer[256];
    double input_value;
    
    printf("Available operations:\n");
    for (int i = 0; i < num_functions; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Enter operation number (1-%d): ", num_functions);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long choice = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid operation number\n");
        return 1;
    }
    if (choice < 1 || choice > num_functions) {
        fprintf(stderr, "Operation number out of range\n");
        return 1;
    }
    
    printf("Enter numeric value: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input(input_buffer, &input_value)) {
        fprintf(stderr, "Invalid numeric value\n");
        return 1;
    }
    
    numeric_func selected_func = functions[choice - 1];
    double result = selected_func(input_value);
    
    printf("Operation: %s\n", names[choice - 1]);
    printf("Input: %.6f\n", input_value);
    print_result(input_value, result);
    
    return 0;
}