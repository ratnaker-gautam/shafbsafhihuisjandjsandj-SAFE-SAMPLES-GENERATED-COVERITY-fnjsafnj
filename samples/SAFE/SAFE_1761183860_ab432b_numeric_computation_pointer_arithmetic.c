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

int validate_input(const char* input, int* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < 1 || val > ARRAY_SIZE) return 0;
    
    *result = (int)val;
    return 1;
}

int main() {
    char buffer[256];
    int count;
    
    printf("Enter number of Fibonacci numbers to generate (1-%d): ", ARRAY_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!validate_input(buffer, &count)) {
        fprintf(stderr, "Invalid input. Please enter an integer between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    int fib_array[ARRAY_SIZE];
    fill_fibonacci(fib_array, count);
    
    printf("First %d Fibonacci numbers:\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d ", *(fib_array + i));
    }
    printf("\n");
    
    double poly_coeffs[] = {2.0, -3.0, 1.0};
    int degree = 2;
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.1f ", *(poly_coeffs + i));
    }
    printf("\n");
    
    printf("Enter x value for polynomial evaluation: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    double x = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    double result = compute_polynomial(poly_coeffs, degree, x);
    printf("Polynomial result at x=%.2f: %.2f\n", x, result);
    
    return 0;
}