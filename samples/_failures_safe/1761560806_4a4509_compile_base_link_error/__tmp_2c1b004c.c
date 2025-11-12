//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    
    double sequence[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    
    for (int i = 0; i < n; i++) {
        sequence[i] = sin((i + 1) * 0.1) + cos((i + 1) * 0.05);
    }
    
    int i = 0;
    while (i < n) {
        sum += sequence[i];
        i++;
    }
    
    int j = 0;
    do {
        if (fabs(sequence[j]) > 1e-10) {
            product *= sequence[j];
        }
        j++;
    } while (j < n);
    
    double mean = sum / n;
    double geometric_mean;
    
    if (product >= 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = -pow(-product, 1.0 / n);
    }
    
    double variance_sum = 0.0;
    for (int k = 0; k < n; k++) {
        double diff = sequence[k] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / n;
    double std_dev = sqrt(variance);
    
    printf("Computed %d terms of sequence\n", n);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    double running_sum = 0.0;
    printf("Running sums:\n");
    for (int idx = 0; idx < n; idx++) {
        running_sum += sequence[idx];
        printf("Term %d: sequence=%.6f, running_sum=%.6f\n", 
               idx + 1, sequence[idx], running_sum);
    }
    
    int count_positive = 0;
    int count_negative = 0;
    for (int idx = 0; idx < n; idx++) {
        if (sequence[idx] > 0) {
            count_positive++;
        } else if (sequence[idx] < 0) {
            count_negative++;
        }
    }
    
    printf("Positive terms: %d\n", count_positive);
    printf("Negative terms: %d\n", count_negative);
    
    return 0;
}