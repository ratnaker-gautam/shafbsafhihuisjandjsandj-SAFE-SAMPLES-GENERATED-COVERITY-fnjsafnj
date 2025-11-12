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
    
    for (int i = 0; i < n; i++) {
        sequence[i] = 1.0 / ((i + 1.0) * (i + 1.0));
    }
    
    int j = 0;
    while (j < n) {
        sum += sequence[j];
        j++;
    }
    
    double pi_approximation = sqrt(6.0 * sum);
    double actual_pi = 3.14159265358979323846;
    
    printf("Computed pi approximation: %.10f\n", pi_approximation);
    printf("Actual pi: %.10f\n", actual_pi);
    printf("Difference: %.10f\n", fabs(pi_approximation - actual_pi));
    
    double running_sum = 0.0;
    int k = 0;
    do {
        if (k < n) {
            running_sum += sequence[k];
            printf("After %d terms: sum = %.10f\n", k + 1, running_sum);
        }
        k++;
    } while (k <= n);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    
    for (int m = 1; m < n; m++) {
        if (sequence[m] > max_val) {
            max_val = sequence[m];
        }
        if (sequence[m] < min_val) {
            min_val = sequence[m];
        }
    }
    
    printf("Maximum term: %.10f\n", max_val);
    printf("Minimum term: %.10f\n", min_val);
    
    int count = 0;
    double threshold = 0.01;
    for (int p = 0; p < n; p++) {
        if (sequence[p] > threshold) {
            count++;
        }
    }
    
    printf("Terms above %.2f: %d\n", threshold, count);
    
    return 0;
}