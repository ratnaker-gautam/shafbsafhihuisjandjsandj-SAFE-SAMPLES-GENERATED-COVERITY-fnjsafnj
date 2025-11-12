//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n;
    double sum = 0.0;
    double mean = 0.0;
    double variance = 0.0;
    double std_dev = 0.0;
    
    printf("Enter number of data points (2-1000): ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (n < 2 || n > 1000) {
        printf("Number must be between 2 and 1000\n");
        return 1;
    }
    
    double *data = (double*)malloc(n * sizeof(double));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lf", &data[i]) != 1) {
            printf("Invalid input\n");
            free(data);
            return 1;
        }
    }
    
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    mean = sum / n;
    
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    variance = sum_sq_diff / (n - 1);
    std_dev = sqrt(variance);
    
    printf("Statistical Analysis:\n");
    printf("Count: %d\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    double min_val = data[0];
    double max_val = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
    }
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    free(data);
    return 0;
}