//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int main(void) {
    double data[MAX_SIZE];
    double *ptr = data;
    double *end_ptr = data + MAX_SIZE;
    int count = 0;
    
    printf("Enter up to %d numbers (non-number to stop): ", MAX_SIZE);
    
    while (ptr < end_ptr) {
        double value;
        if (scanf("%lf", &value) != 1) {
            break;
        }
        
        if (!isfinite(value)) {
            printf("Invalid input: non-finite number\n");
            return 1;
        }
        
        *ptr = value;
        ptr++;
        count++;
        
        if (count >= MAX_SIZE) {
            break;
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Entered %d numbers\n", count);
    
    double sum = 0.0;
    double *sum_ptr = data;
    while (sum_ptr < data + count) {
        sum += *sum_ptr;
        sum_ptr++;
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    double *var_ptr = data;
    while (var_ptr < data + count) {
        double diff = *var_ptr - mean;
        variance_sum += diff * diff;
        var_ptr++;
    }
    
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    double min_val = data[0];
    double max_val = data[0];
    double *minmax_ptr = data + 1;
    while (minmax_ptr < data + count) {
        if (*minmax_ptr < min_val) {
            min_val = *minmax_ptr;
        }
        if (*minmax_ptr > max_val) {
            max_val = *minmax_ptr;
        }
        minmax_ptr++;
    }
    
    printf("Statistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    double target;
    printf("Enter a number to search for: ");
    if (scanf("%lf", &target) == 1) {
        int found = 0;
        double *search_ptr = data;
        while (search_ptr < data + count) {
            if (fabs(*search_ptr - target) < 1e-10) {
                found = 1;
                break;
            }
            search_ptr++;
        }
        
        if (found) {
            printf("Value %.6f found in the dataset\n", target);
        } else {
            printf("Value %.6f not found in the dataset\n", target);
        }
    } else {
        printf("Invalid search value\n");
    }
    
    return 0;
}