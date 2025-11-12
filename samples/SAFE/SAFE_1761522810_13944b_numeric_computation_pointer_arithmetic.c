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

void fill_fibonacci(int* arr, int count) {
    if (count < 1 || arr == NULL) return;
    
    int* ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        if (i < ARRAY_SIZE) {
            *(arr + i) = *(arr + i - 1) + *(arr + i - 2);
        }
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
    double coefficients[5] = {2.0, -3.0, 1.5, -0.5, 1.0};
    int degree = 4;
    
    printf("Polynomial evaluation: f(x) = 2.0 - 3.0x + 1.5x^2 - 0.5x^3 + 1.0x^4\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, degree, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence generation:\n");
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, 20);
    
    int* fib_ptr = fibonacci;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
        if ((i + 1) % 10 == 0) printf("\n");
    }
    
    printf("\nInteger array manipulation:\n");
    int numbers[ARRAY_SIZE];
    int* num_ptr = numbers;
    
    for (int i = 0; i < 10; i++) {
        *(num_ptr + i) = i * i;
    }
    
    int* start = numbers;
    int* end = numbers + 9;
    
    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    for (int i = 0; i < 10; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    char input_buffer[100];
    printf("\nEnter an integer to compute its factorial: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        int n;
        if (validate_integer_input(input_buffer, &n) && n >= 0 && n <= 12) {
            long factorial = 1;
            for (int i = 1; i <= n; i++) {
                factorial *= i;
            }
            printf("%d! = %ld\n", n, factorial);
        } else {
            printf("Invalid input. Please enter an integer between 0 and 12.\n");
        }
    }
    
    return 0;
}