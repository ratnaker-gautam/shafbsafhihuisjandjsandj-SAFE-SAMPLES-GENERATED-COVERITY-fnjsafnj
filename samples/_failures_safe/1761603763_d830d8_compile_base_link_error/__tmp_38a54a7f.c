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
    int i = 0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    while (i < ARRAY_SIZE) {
        double input;
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid input: non-finite number\n");
            continue;
        }
        
        numbers[i] = input;
        valid_count++;
        i++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    min_val = numbers[0];
    max_val = numbers[0];
    
    for (int j = 0; j < valid_count; j++) {
        sum += numbers[j];
        
        if (numbers[j] != 0.0) {
            if (fabs(product) > 1e300 && fabs(numbers[j]) > 1.0) {
                product = (product > 0) ? INFINITY : -INFINITY;
            } else if (fabs(product) < 1e-300 && fabs(numbers[j]) < 1.0) {
                product = 0.0;
            } else {
                product *= numbers[j];
            }
        } else {
            product = 0.0;
        }
        
        if (numbers[j] < min_val) min_val = numbers[j];
        if (numbers[j] > max_val) max_val = numbers[j];
    }
    
    double mean = sum / valid_count;
    double variance = 0.0;
    
    int k = 0;
    do {
        double diff = numbers[k] - mean;
        variance += diff * diff;
        k++;
    } while (k < valid_count);
    
    variance /= valid_count;
    double std_dev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("First 10 numbers squared:\n");
    for (int m = 0; m < valid_count && m < 10; m++) {
        printf("%.6f ", numbers[m] * numbers[m]);
    }
    printf("\n");
    
    return 0;
}