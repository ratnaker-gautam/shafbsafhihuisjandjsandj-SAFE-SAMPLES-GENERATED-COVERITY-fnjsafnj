//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double numbers[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    double min_val, max_val;
    int valid_count = 0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid number detected, stopping input.\n");
            break;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    min_val = numbers[0];
    max_val = numbers[0];
    
    int i = 0;
    while (i < valid_count) {
        sum += numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
        i++;
    }
    
    int j = 0;
    do {
        if (fabs(numbers[j]) > 1e-300) {
            product *= numbers[j];
        }
        j++;
    } while (j < valid_count);
    
    double mean = sum / valid_count;
    double variance = 0.0;
    
    for (int k = 0; k < valid_count; k++) {
        double diff = numbers[k] - mean;
        variance += diff * diff;
    }
    variance /= valid_count;
    double std_dev = sqrt(variance);
    
    double harmonic_sum = 0.0;
    int m = 0;
    while (m < valid_count) {
        if (fabs(numbers[m]) > 1e-10) {
            harmonic_sum += 1.0 / numbers[m];
        }
        m++;
    }
    double harmonic_mean = valid_count / harmonic_sum;
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double approx_pi = 0.0;
    int n;
    for (n = 0; n < MAX_ITERATIONS; n++) {
        double term = (n % 2 == 0) ? 1.0 : -1.0;
        term /= (2 * n + 1);
        approx_pi += term;
        
        if (fabs(term) < 1e-10) {
            break;
        }
    }
    approx_pi *= 4;
    
    printf("Pi approximation (%d terms): %.10f\n", n, approx_pi);
    
    return 0;
}