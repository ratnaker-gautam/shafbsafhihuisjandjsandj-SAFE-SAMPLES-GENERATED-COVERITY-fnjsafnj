//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    while (ptr >= coeffs) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

void fill_coefficients(double *arr, int size) {
    for (int i = 0; i < size; i++) {
        *(arr + i) = (double)(i + 1) / 10.0;
    }
}

int read_positive_int(const char *prompt, int max_value) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value <= 0) {
            printf("Value must be positive.\n");
            continue;
        }
        
        if (value > max_value) {
            printf("Value must not exceed %d.\n", max_value);
            continue;
        }
        
        return value;
    }
}

double read_double(const char *prompt) {
    char buffer[32];
    double value;
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0.0;
        }
        
        if (sscanf(buffer, "%lf", &value) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        return value;
    }
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    int degree;
    double x_value, result;
    
    fill_coefficients(coefficients, ARRAY_SIZE);
    
    printf("Polynomial Evaluation Calculator\n");
    printf("===============================\n\n");
    
    degree = read_positive_int("Enter polynomial degree (1-99): ", ARRAY_SIZE - 1);
    if (degree < 0) {
        return 1;
    }
    
    x_value = read_double("Enter x value for evaluation: ");
    
    printf("\nCoefficients used:\n");
    for (int i = 0; i <= degree; i++) {
        printf("a%d = %.3f\n", i, *(coefficients + i));
    }
    
    result = compute_polynomial(coefficients, degree, x_value);
    
    printf("\nResult: P(%.3f) = %.6f\n", x_value, result);
    
    return 0;
}