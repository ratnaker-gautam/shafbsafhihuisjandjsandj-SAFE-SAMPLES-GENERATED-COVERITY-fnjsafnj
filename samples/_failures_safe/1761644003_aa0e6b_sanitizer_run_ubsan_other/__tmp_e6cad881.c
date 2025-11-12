//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    return result;
}

void fill_fibonacci(int *arr, int count) {
    if (count < 1) return;
    
    int *ptr = arr;
    *ptr = 0;
    if (count > 1) {
        ptr++;
        *ptr = 1;
    }
    
    for (int i = 2; i < count; i++) {
        ptr = arr + i;
        *ptr = *(ptr - 1) + *(ptr - 2);
    }
}

int validate_input(const char *input, double *result) {
    if (input == NULL) return 0;
    
    char *endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (*result == HUGE_VAL || *result == -HUGE_VAL) return 0;
    
    return 1;
}

int main(void) {
    double coefficients[5] = {2.5, -1.8, 3.2, -0.5, 1.0};
    int fibonacci[ARRAY_SIZE];
    char input_buffer[256];
    double x_value;
    
    fill_fibonacci(fibonacci, ARRAY_SIZE);
    
    printf("Polynomial coefficients: ");
    double *coeff_ptr = coefficients;
    for (int i = 0; i < 5; i++) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    printf("Enter x value for polynomial evaluation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &x_value)) {
        printf("Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    double poly_result = compute_polynomial(coefficients, 4, x_value);
    printf("Polynomial result at x=%.2f: %.6f\n", x_value, poly_result);
    
    printf("First 20 Fibonacci numbers: ");
    int *fib_ptr = fibonacci;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    printf("Enter Fibonacci index (0-99): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    char *endptr;
    long index = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\0') {
        printf("Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    if (index < 0 || index >= ARRAY_SIZE) {
        printf("Index out of range. Please enter 0-99.\n");
        return 1;
    }
    
    int *result_ptr = fibonacci + index;
    printf("Fibonacci[%ld] = %d\n", index, *result_ptr);
    
    return 0;
}