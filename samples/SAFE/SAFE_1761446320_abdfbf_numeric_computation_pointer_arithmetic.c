//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

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
    if (count < 1 || arr == NULL) return;
    
    int *ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        long long sum = (long long)*(arr + i - 1) + (long long)*(arr + i - 2);
        if (sum > INT_MAX || sum < INT_MIN) {
            *(arr + i) = 0;
        } else {
            *(arr + i) = (int)sum;
        }
    }
}

int validate_integer_input(const char *input, int *value) {
    if (input == NULL || value == NULL) return 0;
    
    char *endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (temp < INT_MIN || temp > INT_MAX) return 0;
    
    *value = (int)temp;
    return 1;
}

int main(void) {
    char input_buffer[256];
    int degree, x_value;
    double coefficients[ARRAY_SIZE];
    
    printf("Enter polynomial degree (0-%d): ", ARRAY_SIZE - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strcspn(input_buffer, "\n");
    if (len >= sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    input_buffer[len] = '\0';
    
    if (!validate_integer_input(input_buffer, &degree)) {
        fprintf(stderr, "Invalid degree\n");
        return 1;
    }
    
    if (degree < 0 || degree >= ARRAY_SIZE) {
        fprintf(stderr, "Degree out of range\n");
        return 1;
    }
    
    printf("Enter %d coefficients:\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        len = strcspn(input_buffer, "\n");
        if (len >= sizeof(input_buffer)) {
            fprintf(stderr, "Input too long\n");
            return 1;
        }
        input_buffer[len] = '\0';
        
        char *endptr;
        coefficients[i] = strtod(input_buffer, &endptr);
        if (endptr == input_buffer || *endptr != '\0') {
            fprintf(stderr, "Invalid coefficient\n");
            return 1;
        }
    }
    
    printf("Enter x value: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    len = strcspn(input_buffer, "\n");
    if (len >= sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    input_buffer[len] = '\0';
    
    if (!validate_integer_input(input_buffer, &x_value)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, (double)x_value);
    printf("Polynomial result: %.6f\n", result);
    
    int fibonacci_count;
    printf("Enter Fibonacci sequence length (1-%d): ", ARRAY_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    len = strcspn(input_buffer, "\n");
    if (len >= sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    input_buffer[len] = '\0';
    
    if (!validate_integer_input(input_buffer, &fibonacci_count)) {
        fprintf(stderr, "Invalid Fibonacci count\n");
        return 1;
    }
    
    if (fibonacci_count < 1 || fibonacci_count > ARRAY_SIZE) {
        fprintf(stderr, "Fibonacci count out of range\n");
        return 1;
    }
    
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, fibonacci_count);
    
    printf("Fibonacci sequence: ");
    for (int i = 0; i < fibonacci_count; i++) {
        printf("%d ", *(fibonacci + i));
    }
    printf("\n");
    
    return 0;
}