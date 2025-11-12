//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT_LEN 32

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    double x, result = 0.0;
    int n, i, j;
    
    printf("Enter a number between -10 and 10: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (x < -10.0 || x > 10.0) {
        fprintf(stderr, "Number out of range\n");
        return 1;
    }
    
    printf("Enter number of terms (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Term count out of range\n");
        return 1;
    }
    
    printf("Computing e^%.2f using %d terms...\n", x, n);
    
    i = 0;
    while (i < n) {
        double term = 1.0;
        double factorial = 1.0;
        
        j = 1;
        while (j <= i) {
            if (factorial > 1e308 / j) {
                fprintf(stderr, "Factorial overflow\n");
                return 1;
            }
            factorial *= j;
            j++;
        }
        
        term = pow(x, i) / factorial;
        result += term;
        i++;
    }
    
    double reference = exp(x);
    double error = fabs(result - reference);
    
    printf("Approximation: %.10f\n", result);
    printf("Reference:     %.10f\n", reference);
    printf("Absolute error: %.2e\n", error);
    
    printf("\nComputing using for loop...\n");
    double result2 = 0.0;
    
    for (int k = 0; k < n; k++) {
        double factorial = 1.0;
        
        for (int m = 1; m <= k; m++) {
            if (factorial > 1e308 / m) {
                fprintf(stderr, "Factorial overflow\n");
                return 1;
            }
            factorial *= m;
        }
        
        result2 += pow(x, k) / factorial;
    }
    
    printf("For loop result: %.10f\n", result2);
    printf("Results match: %s\n", fabs(result - result2) < 1e-10 ? "yes" : "no");
    
    return 0;
}