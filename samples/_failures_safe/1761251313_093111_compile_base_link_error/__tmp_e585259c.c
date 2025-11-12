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
            printf("Invalid input: non-finite number\n");
            return 1;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered\n");
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
    
    double temp_product = 1.0;
    for (int j = 0; j < valid_count; j++) {
        if (fabs(numbers[j]) > 1e100 || fabs(temp_product) > 1e100) {
            printf("Product calculation would overflow\n");
            temp_product = NAN;
            break;
        }
        temp_product *= numbers[j];
    }
    
    if (!isnan(temp_product)) {
        product = temp_product;
    }
    
    double mean = sum / valid_count;
    
    double variance_sum = 0.0;
    int k = 0;
    do {
        double diff = numbers[k] - mean;
        variance_sum += diff * diff;
        k++;
    } while (k < valid_count);
    
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    if (!isnan(product)) {
        printf("Product: %.6f\n", product);
    } else {
        printf("Product: overflow\n");
    }
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nNumbers in reverse order:\n");
    for (int idx = valid_count - 1; idx >= 0; idx--) {
        printf("%.6f ", numbers[idx]);
        if ((valid_count - idx) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    return 0;
}