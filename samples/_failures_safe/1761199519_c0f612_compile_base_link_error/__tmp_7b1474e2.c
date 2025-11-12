//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT];
    int n;
    
    printf("Enter number of terms to compute (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 20) {
        fprintf(stderr, "Number must be between 1 and 20\n");
        return 1;
    }
    
    double sum = 0.0;
    double product = 1.0;
    double harmonic = 0.0;
    
    printf("Computing series up to term %d:\n", n);
    
    int i = 1;
    while (i <= n) {
        double term = 1.0 / i;
        sum += term;
        product *= (1.0 + term);
        harmonic += term;
        printf("Term %d: 1/%d = %.6f\n", i, i, term);
        i++;
    }
    
    double geometric_mean = exp(log(product) / n);
    double arithmetic_mean = sum / n;
    double harmonic_mean = n / harmonic;
    
    printf("\nResults:\n");
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Geometric mean:  %.6f\n", geometric_mean);
    printf("Harmonic mean:   %.6f\n", harmonic_mean);
    
    double min_val = arithmetic_mean;
    double max_val = arithmetic_mean;
    
    if (geometric_mean < min_val) min_val = geometric_mean;
    if (geometric_mean > max_val) max_val = geometric_mean;
    if (harmonic_mean < min_val) min_val = harmonic_mean;
    if (harmonic_mean > max_val) max_val = harmonic_mean;
    
    printf("Minimum mean:    %.6f\n", min_val);
    printf("Maximum mean:    %.6f\n", max_val);
    
    double range = max_val - min_val;
    printf("Range:           %.6f\n", range);
    
    double running_sum = 0.0;
    printf("\nRunning sum of means: ");
    for (int j = 0; j < 3; j++) {
        double current_mean;
        switch (j) {
            case 0: current_mean = arithmetic_mean; break;
            case 1: current_mean = geometric_mean; break;
            case 2: current_mean = harmonic_mean; break;
            default: current_mean = 0.0;
        }
        running_sum += current_mean;
        printf("%.6f", running_sum);
        if (j < 2) printf(" -> ");
    }
    printf("\n");
    
    double final_result = (arithmetic_mean + geometric_mean + harmonic_mean) / 3.0;
    printf("Final combined result: %.6f\n", final_result);
    
    return 0;
}