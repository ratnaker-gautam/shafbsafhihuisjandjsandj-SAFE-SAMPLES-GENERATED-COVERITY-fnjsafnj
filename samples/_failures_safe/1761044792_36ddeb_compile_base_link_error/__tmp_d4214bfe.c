//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid input: non-finite number\n");
            continue;
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
        
        if (fabs(product) > 1e308 / fabs(input)) {
            product = 0.0;
        } else {
            product *= input;
        }
        
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double mean = sum / valid_count;
    double variance_sum = 0.0;
    
    int i = 0;
    while (i < valid_count) {
        double diff = numbers[i] - mean;
        variance_sum += diff * diff;
        i++;
    }
    
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    double harmonic_sum = 0.0;
    int j = 0;
    do {
        if (fabs(numbers[j]) > 1e-10) {
            harmonic_sum += 1.0 / numbers[j];
        }
        j++;
    } while (j < valid_count);
    
    double harmonic_mean = 0.0;
    if (fabs(harmonic_sum) > 1e-10) {
        harmonic_mean = valid_count / harmonic_sum;
    }
    
    double geometric_mean = 0.0;
    if (product > 0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    }
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    if (harmonic_mean > 0) {
        printf("Harmonic Mean: %.6f\n", harmonic_mean);
    }
    
    if (geometric_mean > 0) {
        printf("Geometric Mean: %.6f\n", geometric_mean);
    }
    
    printf("\nNumber Analysis:\n");
    int even_count = 0;
    int odd_count = 0;
    int positive_count = 0;
    int negative_count = 0;
    int zero_count = 0;
    
    for (int k = 0; k < valid_count; k++) {
        if (fmod(numbers[k], 2.0) == 0.0) {
            even_count++;
        } else {
            odd_count++;
        }
        
        if (numbers[k] > 0) {
            positive_count++;
        } else if (numbers[k] < 0) {
            negative_count++;
        } else {
            zero_count++;
        }
    }
    
    printf("Even numbers: %d\n", even_count);
    printf("Odd numbers: %d\n", odd_count);
    printf("Positive numbers: %d\n", positive_count);
    printf("Negative numbers: %d\n", negative_count);
    printf("Zero values: %d\n", zero_count);
    
    return 0;
}