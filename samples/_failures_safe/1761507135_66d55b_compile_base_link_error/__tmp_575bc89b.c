//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double numbers[ARRAY_SIZE];
    int valid_count = 0;
    double sum = 0.0;
    double product = 1.0;
    double min_val = 0.0;
    double max_val = 0.0;
    int first_valid = 1;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        printf("Number %d: ", i + 1);
        
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid number (infinity/NaN). Stopping input.\n");
            break;
        }
        
        numbers[valid_count] = input;
        
        if (first_valid) {
            min_val = input;
            max_val = input;
            first_valid = 0;
        } else {
            if (input < min_val) min_val = input;
            if (input > max_val) max_val = input;
        }
        
        sum += input;
        
        if (fabs(input) < 1e-10) {
            product = 0.0;
        } else if (product != 0.0) {
            product *= input;
        }
        
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("Numbers entered: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    
    double mean = sum / valid_count;
    printf("Mean: %.6f\n", mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    double variance_sum = 0.0;
    int j = 0;
    while (j < valid_count) {
        double diff = numbers[j] - mean;
        variance_sum += diff * diff;
        j++;
    }
    double variance = variance_sum / valid_count;
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    double geometric_mean;
    if (product == 0.0) {
        geometric_mean = 0.0;
    } else {
        geometric_mean = pow(fabs(product), 1.0 / valid_count);
        if (product < 0) geometric_mean = -geometric_mean;
    }
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double harmonic_sum = 0.0;
    int k = 0;
    do {
        if (fabs(numbers[k]) > 1e-10) {
            harmonic_sum += 1.0 / numbers[k];
        }
        k++;
    } while (k < valid_count);
    
    double harmonic_mean;
    if (fabs(harmonic_sum) < 1e-10) {
        harmonic_mean = 0.0;
    } else {
        harmonic_mean = valid_count / harmonic_sum;
    }
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    int positive_count = 0;
    int negative_count = 0;
    int zero_count = 0;
    
    for (int m = 0; m < valid_count; m++) {
        if (numbers[m] > 1e-10) {
            positive_count++;
        } else if (numbers[m] < -1e-10) {
            negative_count++;
        } else {
            zero_count++;
        }
    }
    
    printf("Positive numbers: %d\n", positive_count);
    printf("Negative numbers: %d\n", negative_count);
    printf("Zero numbers: %d\n", zero_count);
    
    return 0;
}