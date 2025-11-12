//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

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
    if (size == 1) return;
    
    ptr++;
    *ptr = 1;
    if (size == 2) return;
    
    for (int i = 2; i < size; i++) {
        *(arr + i) = *(arr + (i - 1)) + *(arr + (i - 2));
    }
}

int validate_integer_input(const char* input, int* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

int main() {
    int choice;
    char input_buffer[256];
    
    printf("Numeric Computation Demo\n");
    printf("1. Polynomial Evaluation\n");
    printf("2. Fibonacci Sequence\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (!validate_integer_input(input_buffer, &choice)) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            double coeffs[10];
            int degree;
            double x;
            
            printf("Enter polynomial degree (0-9): ");
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            if (!validate_integer_input(input_buffer, &degree) || degree < 0 || degree > 9) {
                fprintf(stderr, "Invalid degree\n");
                return 1;
            }
            
            printf("Enter %d coefficients:\n", degree + 1);
            for (int i = 0; i <= degree; i++) {
                printf("Coefficient %d: ", i);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    fprintf(stderr, "Input error\n");
                    return 1;
                }
                char* endptr;
                coeffs[i] = strtod(input_buffer, &endptr);
                if (endptr == input_buffer || *endptr != '\n') {
                    fprintf(stderr, "Invalid coefficient\n");
                    return 1;
                }
            }
            
            printf("Enter x value: ");
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            char* endptr;
            x = strtod(input_buffer, &endptr);
            if (endptr == input_buffer || *endptr != '\n') {
                fprintf(stderr, "Invalid x value\n");
                return 1;
            }
            
            double result = compute_polynomial(coeffs, degree, x);
            printf("Polynomial result: %.6f\n", result);
            break;
        }
        
        case 2: {
            int count;
            printf("Enter Fibonacci count (1-%d): ", ARRAY_SIZE);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                fprintf(stderr, "Input error\n");
                return 1;
            }
            if (!validate_integer_input(input_buffer, &count) || count < 1 || count > ARRAY_SIZE) {
                fprintf(stderr, "Invalid count\n");
                return 1;
            }
            
            int fib_array[ARRAY_SIZE];
            fill_fibonacci(fib_array, count);
            
            printf("Fibonacci sequence:\n");
            for (int i = 0; i < count; i++) {
                printf("%d ", *(fib_array + i));
            }
            printf("\n");
            break;
        }
        
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}