//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
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
    
    for (int i = 0; i < n; i++) {
        results[i] = sin((double)i * 0.1) + cos((double)i * 0.05);
    }
    
    int j = 0;
    while (j < n) {
        sum += results[j];
        j++;
    }
    
    int k = 0;
    do {
        if (fabs(results[k]) > 1e-10) {
            product *= results[k];
        }
        k++;
    } while (k < n);
    
    double mean = sum / n;
    double geometric_mean = pow(fabs(product), 1.0 / n);
    
    printf("Results for %d iterations:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double min_val = results[0];
    double max_val = results[0];
    for (int i = 1; i < n; i++) {
        if (results[i] < min_val) min_val = results[i];
        if (results[i] > max_val) max_val = results[i];
    }
    
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = results[i] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    return 0;
}