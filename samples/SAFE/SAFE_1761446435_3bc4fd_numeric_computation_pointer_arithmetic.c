//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double* coeffs, int degree, double x) {
    double result = 0.0;
    double* ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    return result;
}

void fill_fibonacci(int* arr, int count) {
    if (count < 1) return;
    
    int* ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        ptr = arr + i;
        *ptr = *(ptr - 1) + *(ptr - 2);
    }
}

int validate_input(const char* input, int* value) {
    if (input == NULL) return 0;
    
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (temp < 1 || temp > ARRAY_SIZE) return 0;
    
    *value = (int)temp;
    return 1;
}

int main() {
    double coefficients[ARRAY_SIZE];
    int fibonacci[ARRAY_SIZE];
    
    printf("Enter number of polynomial coefficients (1-%d): ", ARRAY_SIZE);
    char input_buffer[256];
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    int degree;
    if (!validate_input(input_buffer, &degree)) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    printf("Enter %d coefficients (from highest to lowest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        char* endptr;
        double value = strtod(input_buffer, &endptr);
        if (endptr == input_buffer || *endptr != '\n') {
            fprintf(stderr, "Invalid coefficient\n");
            return 1;
        }
        
        *(coefficients + i) = value;
    }
    
    printf("Enter x value for polynomial evaluation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    double x_value = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    printf("Polynomial result at x=%.2f: %.6f\n", x_value, result);
    
    fill_fibonacci(fibonacci, degree + 1);
    printf("First %d Fibonacci numbers:\n", degree + 1);
    
    int* fib_ptr = fibonacci;
    for (int i = 0; i <= degree; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    printf("Array statistics:\n");
    double sum = 0.0;
    double* coeff_ptr = coefficients;
    
    for (int i = 0; i <= degree; i++) {
        sum += *coeff_ptr;
        coeff_ptr++;
    }
    
    double mean = sum / (degree + 1);
    printf("Mean of coefficients: %.6f\n", mean);
    
    double variance_sum = 0.0;
    coeff_ptr = coefficients;
    
    for (int i = 0; i <= degree; i++) {
        double diff = *coeff_ptr - mean;
        variance_sum += diff * diff;
        coeff_ptr++;
    }
    
    double variance = variance_sum / (degree + 1);
    printf("Variance of coefficients: %.6f\n", variance);
    
    return 0;
}