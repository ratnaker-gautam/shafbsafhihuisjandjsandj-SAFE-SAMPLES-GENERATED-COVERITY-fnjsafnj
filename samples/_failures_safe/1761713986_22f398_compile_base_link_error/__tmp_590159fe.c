//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    int valid_count = 0;
    
    printf("Enter %d numbers between -1000 and 1000:\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        printf("Number %d: ", i + 1);
        
        if (scanf("%lf", &input) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }
        
        if (input < -1000.0 || input > 1000.0) {
            printf("Number out of range. Please enter between -1000 and 1000.\n");
            i--;
            continue;
        }
        
        results[i] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    int i = 0;
    while (i < valid_count) {
        sum += results[i];
        i++;
    }
    
    double temp_product = 1.0;
    int j = 0;
    do {
        if (results[j] != 0.0) {
            if (fabs(temp_product) > 1e308 / fabs(results[j])) {
                printf("Product would overflow, setting to safe value.\n");
                temp_product = 0.0;
                break;
            }
            temp_product *= results[j];
        }
        j++;
    } while (j < valid_count);
    product = temp_product;
    
    double mean = sum / valid_count;
    
    double variance_sum = 0.0;
    for (int k = 0; k < valid_count; k++) {
        double diff = results[k] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    double geometric_mean;
    if (product >= 0.0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    } else {
        geometric_mean = -pow(-product, 1.0 / valid_count);
    }
    
    printf("\nStatistical Analysis Results:\n");
    printf("Valid numbers processed: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    printf("\nComputing series approximations...\n");
    double series_results[10];
    
    for (int m = 0; m < 10; m++) {
        double term_sum = 0.0;
        int n = 0;
        while (n < MAX_ITERATIONS) {
            double term = pow(-1, n) / (2.0 * n + 1.0) * pow(0.5, 2 * n + 1);
            if (fabs(term) < 1e-15) {
                break;
            }
            term_sum += term;
            n++;
        }
        series_results[m] = 4.0 * term_sum;
    }
    
    double series_avg = 0.0;
    int idx = 0;
    do {
        series_avg += series_results[idx];
        idx++;
    } while (idx < 10);
    series_avg /= 10.0;
    
    printf("Average of 10 pi/4 series approximations: %.10f\n", series_avg);
    printf("Actual pi/4: %.10f\n", 0.7853981633974483);
    printf("Absolute error: %.10f\n", fabs(series_avg - 0.7853981633974483));
    
    return 0;
}