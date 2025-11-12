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
        fprintf(stderr, "Error: Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    double arr[MAX_SIZE];
    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lf", &arr[i]) != 1) {
            fprintf(stderr, "Error: Invalid number input\n");
            return 1;
        }
    }
    
    double sum = 0.0;
    double min_val = arr[0];
    double max_val = arr[0];
    
    int i = 0;
    while (i < n) {
        sum += arr[i];
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        i++;
    }
    
    double mean = sum / n;
    
    double variance = 0.0;
    for (int j = 0; j < n; j++) {
        double diff = arr[j] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", n);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    
    printf("\nRunning totals:\n");
    double running_sum = 0.0;
    int k = 0;
    do {
        if (k >= n) break;
        running_sum += arr[k];
        printf("After element %d: sum = %.6f\n", k + 1, running_sum);
        k++;
    } while (k < n);
    
    printf("\nElements greater than mean:\n");
    int count_above = 0;
    for (int idx = 0; idx < n; idx++) {
        if (arr[idx] > mean) {
            printf("Element %d: %.6f\n", idx + 1, arr[idx]);
            count_above++;
        }
    }
    printf("Total elements above mean: %d\n", count_above);
    
    return 0;
}