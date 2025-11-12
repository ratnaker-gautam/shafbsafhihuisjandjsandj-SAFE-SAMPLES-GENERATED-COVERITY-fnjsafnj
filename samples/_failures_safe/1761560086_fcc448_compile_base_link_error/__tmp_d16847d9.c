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
    
    double median;
    for (int k = 0; k < n - 1; k++) {
        for (int m = 0; m < n - k - 1; m++) {
            if (data[m] > data[m + 1]) {
                double temp = data[m];
                data[m] = data[m + 1];
                data[m + 1] = temp;
            }
        }
    }
    
    if (n % 2 == 0) {
        median = (data[n/2 - 1] + data[n/2]) / 2.0;
    } else {
        median = data[n/2];
    }
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", n);
    printf("Mean: %.6f\n", mean);
    printf("Median: %.6f\n", median);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    free(data);
    return 0;
}