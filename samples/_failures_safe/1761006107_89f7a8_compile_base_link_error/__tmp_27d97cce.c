//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n;
    double sum = 0.0;
    double product = 1.0;
    double mean, geometric_mean;
    int valid_input = 0;
    
    printf("Enter number of elements (2-100): ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (n < 2 || n > 100) {
        printf("Number of elements must be between 2 and 100.\n");
        return 1;
    }
    
    double *numbers = malloc(n * sizeof(double));
    if (numbers == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    printf("Enter %d positive numbers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Element %d: ", i + 1);
        if (scanf("%lf", &numbers[i]) != 1) {
            printf("Invalid number format.\n");
            free(numbers);
            return 1;
        }
        
        if (numbers[i] <= 0.0) {
            printf("Numbers must be positive.\n");
            free(numbers);
            return 1;
        }
        
        if (numbers[i] > 1e308) {
            printf("Number too large.\n");
            free(numbers);
            return 1;
        }
        
        sum += numbers[i];
        
        if (product > 1e308 / numbers[i]) {
            printf("Product would overflow.\n");
            free(numbers);
            return 1;
        }
        product *= numbers[i];
    }
    
    mean = sum / n;
    geometric_mean = pow(product, 1.0 / n);
    
    printf("\nResults:\n");
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Difference: %.6f\n", fabs(mean - geometric_mean));
    
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    free(numbers);
    return 0;
}