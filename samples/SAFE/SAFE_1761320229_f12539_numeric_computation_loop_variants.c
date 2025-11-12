//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int main() {
    int n;
    printf("Enter number of elements (1-%d): ", MAX_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_SIZE) {
        fprintf(stderr, "Error: Size out of range\n");
        return 1;
    }
    
    double arr[MAX_SIZE];
    printf("Enter %d numbers:\n", n);
    
    int i = 0;
    while (i < n) {
        if (scanf("%lf", &arr[i]) != 1) {
            fprintf(stderr, "Error: Invalid number\n");
            return 1;
        }
        i++;
    }
    
    double sum = 0.0;
    double min_val = arr[0];
    double max_val = arr[0];
    
    for (i = 0; i < n; i++) {
        sum += arr[i];
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }
    
    double mean = sum / n;
    
    double variance = 0.0;
    int j = 0;
    do {
        double diff = arr[j] - mean;
        variance += diff * diff;
        j++;
    } while (j < n);
    variance /= n;
    
    printf("Statistics:\n");
    printf("Count: %d\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Variance: %.6f\n", variance);
    
    printf("Squares: ");
    for (int k = 0; k < n; k++) {
        printf("%.6f", arr[k] * arr[k]);
        if (k < n - 1) printf(", ");
    }
    printf("\n");
    
    printf("Cumulative sums: ");
    double running_sum = 0.0;
    int m = 0;
    while (m < n) {
        running_sum += arr[m];
        printf("%.6f", running_sum);
        if (m < n - 1) printf(", ");
        m++;
    }
    printf("\n");
    
    return 0;
}