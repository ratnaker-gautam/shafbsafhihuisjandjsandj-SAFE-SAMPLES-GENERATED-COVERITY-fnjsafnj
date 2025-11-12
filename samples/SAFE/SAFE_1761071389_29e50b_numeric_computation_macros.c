//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            exit(EXIT_FAILURE);
        }
        
        char* endptr;
        value = strtod(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value > MAX_INPUT || value < MIN_INPUT) {
            printf("Input out of range. Please enter a value between %.0f and %.0f.\n", 
                   MIN_INPUT, MAX_INPUT);
            continue;
        }
        
        if (value != value) {
            printf("Invalid floating point value.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

double newton_sqrt(double n) {
    if (n < 0.0) {
        fprintf(stderr, "Cannot compute square root of negative number\n");
        exit(EXIT_FAILURE);
    }
    
    if (n == 0.0) {
        return 0.0;
    }
    
    double x = n;
    double prev_x;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        prev_x = x;
        x = 0.5 * (x + n / x);
        
        if (fabs(x - prev_x) < EPSILON) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached\n");
        exit(EXIT_FAILURE);
    }
    
    return x;
}

int main(void) {
    printf("Numerical Square Root Calculator\n");
    printf("===============================\n\n");
    
    double number = safe_input("Enter a non-negative number: ");
    
    if (number < 0.0) {
        fprintf(stderr, "Cannot compute square root of negative number\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = newton_sqrt(number);
    double std_sqrt = sqrt(number);
    
    printf("\nResults:\n");
    printf("Newton's method: %.10f\n", sqrt_result);
    printf("Standard library: %.10f\n", std_sqrt);
    printf("Difference: %.10e\n", fabs(sqrt_result - std_sqrt));
    
    if (fabs(sqrt_result - std_sqrt) > EPSILON) {
        fprintf(stderr, "Warning: Significant difference between implementations\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}