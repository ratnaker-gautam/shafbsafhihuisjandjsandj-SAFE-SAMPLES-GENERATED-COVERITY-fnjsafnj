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

void fill_fibonacci(int* arr, int size) {
    if (size < 1) return;
    
    int* ptr = arr;
    *ptr = 0;
    if (size > 1) {
        ptr++;
        *ptr = 1;
    }
    
    for (int i = 2; i < size; i++) {
        ptr = arr + i;
        *ptr = *(ptr - 1) + *(ptr - 2);
    }
}

int validate_input(const char* input, double* result) {
    if (input == NULL) return 0;
    
    char* endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (*result == HUGE_VAL || *result == -HUGE_VAL) {
        return 0;
    }
    
    return 1;
}

int main() {
    double coefficients[] = {2.0, -3.0, 1.0, 5.0};
    int degree = 3;
    int fibonacci[ARRAY_SIZE];
    
    fill_fibonacci(fibonacci, ARRAY_SIZE);
    
    printf("Fibonacci sequence (first %d terms):\n", ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", fibonacci[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n\n");
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.1f ", coefficients[i]);
    }
    printf("\n");
    
    char input_buffer[256];
    printf("Enter a value for x: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    double x_value;
    if (!validate_input(input_buffer, &x_value)) {
        printf("Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    printf("P(%.2f) = %.2f\n", x_value, result);
    
    printf("\nFibonacci numbers at prime indices (up to %d):\n", ARRAY_SIZE);
    int count = 0;
    for (int i = 2; i < ARRAY_SIZE; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf("F[%d] = %d\n", i, fibonacci[i]);
            count++;
            if (count >= 10) break;
        }
    }
    
    return 0;
}