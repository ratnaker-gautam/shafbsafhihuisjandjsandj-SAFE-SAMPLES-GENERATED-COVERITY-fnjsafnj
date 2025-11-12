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
        
        if (value < -1e100 || value > 1e100) {
            printf("Value out of range\n");
            continue;
        }
        
        *ptr = value;
        ptr++;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered\n");
        return 1;
    }
    
    double *current = data;
    double *last = data + count - 1;
    
    printf("Original numbers: ");
    while (current <= last) {
        printf("%.2f ", *current);
        current++;
    }
    printf("\n");
    
    double sum = 0.0;
    double *sum_ptr = data;
    while (sum_ptr < data + count) {
        sum += *sum_ptr;
        sum_ptr++;
    }
    
    double mean = sum / count;
    printf("Mean: %.2f\n", mean);
    
    double variance = 0.0;
    double *var_ptr = data;
    while (var_ptr < data + count) {
        double diff = *var_ptr - mean;
        variance += diff * diff;
        var_ptr++;
    }
    variance /= count;
    
    double std_dev = sqrt(variance);
    printf("Standard deviation: %.2f\n", std_dev);
    
    double *min_ptr = data;
    double *max_ptr = data;
    double *search_ptr = data + 1;
    
    while (search_ptr < data + count) {
        if (*search_ptr < *min_ptr) {
            min_ptr = search_ptr;
        }
        if (*search_ptr > *max_ptr) {
            max_ptr = search_ptr;
        }
        search_ptr++;
    }
    
    printf("Minimum: %.2f\n", *min_ptr);
    printf("Maximum: %.2f\n", *max_ptr);
    
    double *reverse_ptr = data + count - 1;
    printf("Reversed: ");
    while (reverse_ptr >= data) {
        printf("%.2f ", *reverse_ptr);
        reverse_ptr--;
    }
    printf("\n");
    
    return 0;
}