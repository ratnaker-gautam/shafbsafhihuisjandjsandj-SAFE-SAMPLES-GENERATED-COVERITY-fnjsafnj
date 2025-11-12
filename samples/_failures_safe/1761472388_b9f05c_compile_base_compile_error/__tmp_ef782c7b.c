//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT 100
#define PI 3.14159265358979323846

int main() {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter number of terms to compute (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (n < 1 || n > 20) {
        fprintf(stderr, "Number must be between 1 and 20\n");
        return 1;
    }
    
    printf("Computing %d terms of sequence:\n", n);
    
    double sum = 0.0;
    double product = 1.0;
    double max_val = -1.0 / 0.0;
    double min_val = 1.0 / 0.0;
    
    printf("Term\tValue\t\tSum\t\tProduct\n");
    printf("----\t-----\t\t---\t\t-------\n");
    
    int i = 0;
    while (i < n) {
        double term = sin((i + 1) * PI / 8.0) + cos((i + 1) * PI / 12.0);
        
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Numerical error in computation\n");
            return 1;
        }
        
        sum += term;
        
        if (fabs(product) > 1e-300 && fabs(term) > 1e-300) {
            product *= term;
        } else {
            product = 0.0;
        }
        
        if (term > max_val) max_val = term;
        if (term < min_val) min_val = term;
        
        printf("%d\t%.6f\t%.6f\t", i + 1, term, sum);
        if (fabs(product) < 1e-300) {
            printf("0.000000\n");
        } else {
            printf("%.6f\n", product);
        }
        
        i++;
    }
    
    printf("\nStatistics:\n");
    printf("Final sum: %.6f\n", sum);
    printf("Final product: ");
    if (fabs(product) < 1e-300) {
        printf("0.000000\n");
    } else {
        printf("%.6f\n", product);
    }
    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);
    
    double mean = sum / n;
    printf("Mean: %.6f\n", mean);
    
    double variance_sum = 0.0;
    int j = 0;
    do {
        if (j >= n) break;
        double term = sin((j + 1) * PI / 8.0) + cos((j + 1) * PI / 12.0);
        double diff = term - mean;
        variance_sum += diff * diff;
        j++;
    } while (j < n);
    
    double variance = variance_sum / n;
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    printf("\nAlternate computation (squares):\n");
    for (int k = 0; k < n; k++) {
        if (k >= n) break;
        double squared = pow(sin((k + 1) * PI / 8.0), 2.0) + pow(cos((k + 1) * PI / 12.0), 2.0);
        printf("Term %d squared sum: %.6f\n", k + 1, squared);
    }
    
    return 0;
}