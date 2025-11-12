//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    int n;
    
    printf("Enter number of iterations (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sum = 0.0;
    double product = 1.0;
    
    int i = 0;
    while (i < n) {
        double term = sin((double)(i + 1) * 0.1);
        results[i] = term;
        sum += term;
        if (fabs(product) < 1e-300 || fabs(term) < 1e-300) {
            product = 0.0;
        } else {
            product *= term;
        }
        i++;
    }
    
    double harmonic_mean = 0.0;
    for (int j = 0; j < n; j++) {
        if (fabs(results[j]) < 1e-10) {
            harmonic_mean = 0.0;
            break;
        }
        harmonic_mean += 1.0 / results[j];
    }
    if (harmonic_mean != 0.0 && n > 0) {
        harmonic_mean = (double)n / harmonic_mean;
    }
    
    double geometric_mean;
    if (product == 0.0 || n == 0) {
        geometric_mean = 0.0;
    } else {
        geometric_mean = pow(fabs(product), 1.0 / (double)n);
        if (product < 0.0 && n % 2 == 1) {
            geometric_mean = -geometric_mean;
        }
    }
    
    double arithmetic_mean = sum / (double)n;
    
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double variance = 0.0;
    int k = 0;
    do {
        if (k < n) {
            double diff = results[k] - arithmetic_mean;
            variance += diff * diff;
            k++;
        }
    } while (k < n);
    
    if (n > 1) {
        variance /= (double)(n - 1);
    } else {
        variance = 0.0;
    }
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    double min_val = results[0];
    double max_val = results[0];
    for (int idx = 1; idx < n; idx++) {
        if (results[idx] < min_val) {
            min_val = results[idx];
        }
        if (results[idx] > max_val) {
            max_val = results[idx];
        }
    }
    
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Range: %.6f\n", max_val - min_val);
    
    return 0;
}