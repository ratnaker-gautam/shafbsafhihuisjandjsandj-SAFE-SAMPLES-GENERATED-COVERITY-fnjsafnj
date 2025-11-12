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
            printf("Invalid number (infinity/NaN), skipping.\n");
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
        
        if (fabs(input) < 1e-300) {
            product = 0.0;
        } else if (fabs(product) < 1e300) {
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
    int j = 0;
    while (j < valid_count) {
        double diff = numbers[j] - mean;
        variance_sum += diff * diff;
        j++;
    }
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    double geometric_mean;
    if (product == 0.0) {
        geometric_mean = 0.0;
    } else {
        geometric_mean = pow(fabs(product), 1.0 / valid_count);
        if (product < 0 && valid_count % 2 == 1) {
            geometric_mean = -geometric_mean;
        }
    }
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Geometric Mean: %.6f\n", geometric_mean);
    
    printf("\nNumber distribution analysis:\n");
    int positive_count = 0;
    int negative_count = 0;
    int zero_count = 0;
    int even_int_count = 0;
    int odd_int_count = 0;
    
    int k = 0;
    do {
        if (numbers[k] > 0) {
            positive_count++;
        } else if (numbers[k] < 0) {
            negative_count++;
        } else {
            zero_count++;
        }
        
        double int_part;
        double frac_part = modf(numbers[k], &int_part);
        if (fabs(frac_part) < 1e-12) {
            long long_val = (long)int_part;
            if (llabs(long_val) % 2 == 0) {
                even_int_count++;
            } else {
                odd_int_count++;
            }
        }
        k++;
    } while (k < valid_count);
    
    printf("Positive numbers: %d\n", positive_count);
    printf("Negative numbers: %d\n", negative_count);
    printf("Zero values: %d\n", zero_count);
    printf("Even integers: %d\n", even_int_count);
    printf("Odd integers: %d\n", odd_int_count);
    
    printf("\nRunning cumulative analysis:\n");
    double running_sum = 0.0;
    double running_product = 1.0;
    for (int m = 0; m < valid_count; m++) {
        running_sum += numbers[m];
        if (fabs(running_product) < 1e300) {
            running_product *= numbers[m];
        }
        printf("After %d numbers: sum=%.6f", m + 1, running_sum);
        if (fabs(running_product) < 1e300) {
            printf(", product=%.6f", running_product);
        } else {
            printf(", product=overflow");
        }
        printf("\n");
    }
    
    return 0;
}