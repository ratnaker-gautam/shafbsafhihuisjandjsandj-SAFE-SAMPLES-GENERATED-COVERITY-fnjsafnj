//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    double results[ARRAY_SIZE];
    
    printf("Computing sequence terms...\n");
    
    int i = 0;
    while (i < n) {
        double term = 0.0;
        int j;
        for (j = 1; j <= MAX_ITERATIONS; j++) {
            double component = 1.0 / (j * j);
            if (term > DBL_MAX - component) {
                fprintf(stderr, "Error: Numerical overflow\n");
                return 1;
            }
            term += component;
        }
        results[i] = term;
        i++;
    }
    
    double sum = 0.0;
    double product = 1.0;
    
    int k;
    for (k = 0; k < n; k++) {
        if (sum > DBL_MAX - results[k]) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
        sum += results[k];
        
        if (fabs(results[k]) > DBL_MAX / fabs(product)) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        product *= results[k];
    }
    
    double harmonic_mean = 0.0;
    int m = 0;
    do {
        if (results[m] == 0.0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        harmonic_mean += 1.0 / results[m];
        m++;
    } while (m < n);
    
    if (harmonic_mean == 0.0) {
        fprintf(stderr, "Error: Harmonic mean undefined\n");
        return 1;
    }
    harmonic_mean = n / harmonic_mean;
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.10f\n", sum);
    printf("Product: %.10f\n", product);
    printf("Harmonic mean: %.10f\n", harmonic_mean);
    
    double running_avg = 0.0;
    printf("Running averages:\n");
    int p;
    for (p = 0; p < n; p++) {
        running_avg = (running_avg * p + results[p]) / (p + 1);
        printf("After term %d: %.10f\n", p + 1, running_avg);
    }
    
    return 0;
}