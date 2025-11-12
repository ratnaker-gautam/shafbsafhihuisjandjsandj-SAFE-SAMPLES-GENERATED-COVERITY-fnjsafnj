//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n;
    double x, sum = 0.0, product = 1.0;
    
    printf("Enter number of terms (1-100): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Error: Number of terms must be between 1 and 100\n");
        return 1;
    }
    
    printf("Enter base value x: ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (x < -100.0 || x > 100.0) {
        fprintf(stderr, "Error: x must be between -100 and 100\n");
        return 1;
    }
    
    printf("Computing series expansion for e^(%.2f) using %d terms...\n", x, n);
    
    for (int i = 0; i < n; i++) {
        double term = 1.0;
        for (int j = 1; j <= i; j++) {
            if (j > 0 && term > 1e308 / fabs(x)) {
                fprintf(stderr, "Error: Numerical overflow\n");
                return 1;
            }
            term *= x / j;
        }
        
        if (sum > 0 && term > 1e308 - sum) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return 1;
        }
        
        sum += term;
        
        if (i > 0) {
            if (fabs(product) > 1e308 / fabs(term)) {
                fprintf(stderr, "Error: Numerical overflow\n");
                return 1;
            }
            product *= term;
        }
    }
    
    double actual_exp = exp(x);
    double error = fabs(sum - actual_exp);
    double relative_error = error / fabs(actual_exp);
    
    printf("Approximation: %.10f\n", sum);
    printf("Actual e^x:    %.10f\n", actual_exp);
    printf("Absolute error: %.2e\n", error);
    printf("Relative error: %.2e\n", relative_error);
    printf("Product of terms: %.10f\n", product);
    
    if (relative_error < 1e-6) {
        printf("Result: High accuracy achieved\n");
    } else if (relative_error < 1e-3) {
        printf("Result: Good accuracy\n");
    } else {
        printf("Result: Low accuracy\n");
    }
    
    return 0;
}