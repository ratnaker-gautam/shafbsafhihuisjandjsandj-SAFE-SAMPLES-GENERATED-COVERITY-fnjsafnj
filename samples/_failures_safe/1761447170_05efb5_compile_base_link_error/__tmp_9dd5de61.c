//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of elements (1-100): ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        printf("Number must be between 1 and 100\n");
        return 1;
    }
    
    double *data = malloc(n * sizeof(double));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Element %d: ", i + 1);
        if (scanf("%lf", &data[i]) != 1) {
            printf("Invalid input\n");
            free(data);
            return 1;
        }
    }
    
    double sum = 0.0;
    int i = 0;
    while (i < n) {
        sum += data[i];
        i++;
    }
    
    double mean = sum / n;
    
    double variance = 0.0;
    i = 0;
    do {
        double diff = data[i] - mean;
        variance += diff * diff;
        i++;
    } while (i < n);
    variance /= n;
    
    double std_dev = sqrt(variance);
    
    double min_val = data[0];
    double max_val = data[0];
    for (int j = 1; j < n; j++) {
        if (data[j] < min_val) {
            min_val = data[j];
        }
        if (data[j] > max_val) {
            max_val = data[j];
        }
    }
    
    double range = max_val - min_val;
    
    double *sorted = malloc(n * sizeof(double));
    if (sorted == NULL) {
        printf("Memory allocation failed\n");
        free(data);
        return 1;
    }
    memcpy(sorted, data, n * sizeof(double));
    
    for (int pass = 0; pass < n - 1; pass++) {
        for (int idx = 0; idx < n - pass - 1; idx++) {
            if (sorted[idx] > sorted[idx + 1]) {
                double temp = sorted[idx];
                sorted[idx] = sorted[idx + 1];
                sorted[idx + 1] = temp;
            }
        }
    }
    
    double median;
    if (n % 2 == 0) {
        median = (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
    } else {
        median = sorted[n/2];
    }
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", n);
    printf("Mean: %.6f\n", mean);
    printf("Median: %.6f\n", median);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Range: %.6f\n", range);
    
    free(data);
    free(sorted);
    return 0;
}