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
        if (i >= ARRAY_SIZE) break;
        *(arr + i) = *(arr + i - 1) + *(arr + i - 2);
    }
}

int validate_integer_input(const char* input, int* value) {
    if (input == NULL || value == NULL) return 0;
    
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (temp < INT_MIN || temp > INT_MAX) return 0;
    
    *value = (int)temp;
    return 1;
}

int main() {
    double coefficients[5] = {2.0, -3.0, 1.5, 0.5, -1.0};
    int degree = 4;
    
    printf("Polynomial evaluation (2.0 - 3.0x + 1.5x^2 + 0.5x^3 - 1.0x^4)\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, degree, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence generation\n");
    char input[32];
    int count;
    
    printf("Enter number of Fibonacci numbers to generate (1-%d): ", ARRAY_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_integer_input(input, &count)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (count < 1 || count > ARRAY_SIZE) {
        printf("Count must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, count);
    
    printf("First %d Fibonacci numbers:\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d ", *(fibonacci + i));
    }
    printf("\n");
    
    printf("\nArray statistics using pointer arithmetic:\n");
    double numbers[10] = {1.5, 2.8, 3.2, 4.1, 5.9, 6.3, 7.8, 8.4, 9.1, 10.5};
    double sum = 0.0;
    double* num_ptr = numbers;
    
    for (int i = 0; i < 10; i++) {
        sum += *num_ptr;
        num_ptr++;
    }
    
    double mean = sum / 10.0;
    double variance = 0.0;
    num_ptr = numbers;
    
    for (int i = 0; i < 10; i++) {
        double diff = *num_ptr - mean;
        variance += diff * diff;
        num_ptr++;
    }
    variance /= 10.0;
    
    printf("Mean: %.3f, Variance: %.3f\n", mean, variance);
    
    return 0;
}