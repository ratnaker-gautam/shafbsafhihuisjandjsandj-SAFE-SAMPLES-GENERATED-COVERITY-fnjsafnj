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
    
    printf("Computing sequence using different loop variants:\n");
    
    int i = 0;
    while (i < n) {
        sequence[i] = sin((double)(i + 1) * 0.1);
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    double product = 1.0;
    int k = 0;
    do {
        if (k < n) {
            product *= (sequence[k] + 1.0);
        }
        k++;
    } while (k < n);
    
    double running_avg[ARRAY_SIZE];
    double current_sum = 0.0;
    for (int m = 0; m < n; m++) {
        current_sum += sequence[m];
        running_avg[m] = current_sum / (m + 1);
    }
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    int idx = 0;
    while (idx < n) {
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
        idx++;
    }
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product (x+1): %.6f\n", product);
    printf("Final running average: %.6f\n", running_avg[n-1]);
    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);
    
    printf("\nFirst 5 sequence values: ");
    for (int p = 0; p < (n < 5 ? n : 5); p++) {
        printf("%.4f ", sequence[p]);
    }
    printf("\n");
    
    return 0;
}