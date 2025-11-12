//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

double compute_polynomial(double* coeffs, int degree, double x) {
    if (degree < 0 || degree >= MAX_SIZE) return 0.0;
    
    double result = 0.0;
    double* ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

int validate_coefficients(const char* input, double* coeffs, int* degree) {
    if (input == NULL || coeffs == NULL || degree == NULL) return 0;
    
    char buffer[512];
    if (strlen(input) >= sizeof(buffer)) return 0;
    strcpy(buffer, input);
    
    char* token = strtok(buffer, " ");
    int count = 0;
    
    while (token != NULL && count < MAX_SIZE) {
        char* endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') return 0;
        if (value < -1e100 || value > 1e100) return 0;
        
        *(coeffs + count) = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    if (count == 0) return 0;
    *degree = count - 1;
    return 1;
}

int main() {
    double coefficients[MAX_SIZE];
    int degree;
    char input[512];
    double x;
    
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_coefficients(input, coefficients, &degree)) {
        printf("Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (scanf("%lf", &x) != 1) {
        printf("Invalid x value\n");
        return 1;
    }
    
    if (x < -1e100 || x > 1e100) {
        printf("X value out of reasonable range\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x);
    
    printf("Polynomial coefficients: ");
    double* ptr = coefficients;
    for (int i = 0; i <= degree; i++) {
        printf("%.2f ", *ptr);
        ptr++;
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    return 0;
}