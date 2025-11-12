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
    
    double results[ARRAY_SIZE];
    
    int i = 0;
    while (i < n) {
        double sum = 0.0;
        for (int j = 1; j <= i + 1; j++) {
            sum += 1.0 / (j * j);
        }
        results[i] = sqrt(6.0 * sum);
        i++;
    }
    
    printf("Approximations of pi using Basel problem:\n");
    int k = 0;
    do {
        printf("Term %d: %.10f\n", k + 1, results[k]);
        k++;
    } while (k < n);
    
    double running_sum = 0.0;
    for (int idx = 0; idx < n; idx++) {
        running_sum += results[idx];
    }
    double average = running_sum / n;
    
    printf("Average of all approximations: %.10f\n", average);
    
    double max_diff = 0.0;
    for (int idx = 0; idx < n - 1; idx++) {
        double diff = fabs(results[idx + 1] - results[idx]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
    
    printf("Maximum difference between consecutive terms: %.10f\n", max_diff);
    
    return 0;
}