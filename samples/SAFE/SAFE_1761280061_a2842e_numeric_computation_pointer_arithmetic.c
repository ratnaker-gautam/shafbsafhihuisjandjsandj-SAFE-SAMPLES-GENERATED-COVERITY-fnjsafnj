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
        if (i > 1 && i < count) {
            long long sum = (long long)*(arr + i - 1) + (long long)*(arr + i - 2);
            if (sum > INT_MAX || sum < INT_MIN) {
                break;
            }
            *(arr + i) = (int)sum;
        }
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
    double coefficients[5] = {2.5, -1.3, 0.7, -0.2, 1.0};
    int fibonacci_sequence[ARRAY_SIZE] = {0};
    
    printf("Polynomial evaluation with coefficients: ");
    for (int i = 0; i < 5; i++) {
        printf("%.1f ", *(coefficients + i));
    }
    printf("\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, 4, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence generation\n");
    printf("Enter number of terms (1-%d): ", ARRAY_SIZE);
    
    char input_buffer[32];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strcspn(input_buffer, "\n");
    if (len < sizeof(input_buffer)) {
        input_buffer[len] = '\0';
    }
    
    int term_count;
    if (!validate_integer_input(input_buffer, &term_count)) {
        fprintf(stderr, "Invalid input: must be an integer\n");
        return 1;
    }
    
    if (term_count < 1 || term_count > ARRAY_SIZE) {
        fprintf(stderr, "Invalid range: must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    fill_fibonacci(fibonacci_sequence, term_count);
    
    printf("First %d Fibonacci numbers:\n", term_count);
    for (int i = 0; i < term_count; i++) {
        printf("%d ", *(fibonacci_sequence + i));
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");
    
    printf("\nArray statistics:\n");
    int* fib_ptr = fibonacci_sequence;
    long long sum = 0;
    int min = *fib_ptr;
    int max = *fib_ptr;
    
    for (int i = 0; i < term_count; i++) {
        sum += *fib_ptr;
        if (*fib_ptr < min) min = *fib_ptr;
        if (*fib_ptr > max) max = *fib_ptr;
        fib_ptr++;
    }
    
    printf("Sum: %lld\n", sum);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    if (term_count > 0) {
        printf("Average: %.2f\n", (double)sum / term_count);
    }
    
    return 0;
}