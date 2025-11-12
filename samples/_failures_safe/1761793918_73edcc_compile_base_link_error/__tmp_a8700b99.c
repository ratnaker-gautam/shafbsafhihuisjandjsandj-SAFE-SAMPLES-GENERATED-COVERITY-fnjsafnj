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
    double min_val = 1.0 / 0.0;
    double max_val = -1.0 / 0.0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        printf("Number %d: ", i + 1);
        
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid: non-finite number\n");
            continue;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    int i = 0;
    while (i < valid_count) {
        sum += numbers[i];
        i++;
    }
    
    double mean = sum / valid_count;
    
    i = 0;
    do {
        if (numbers[i] < min_val) {
            min_val = numbers[i];
        }
        if (numbers[i] > max_val) {
            max_val = numbers[i];
        }
        i++;
    } while (i < valid_count);
    
    for (i = 0; i < valid_count; i++) {
        if (fabs(numbers[i]) < 1e-300) {
            printf("Warning: very small number detected, product may be imprecise\n");
        }
        product *= numbers[i];
    }
    
    double variance = 0.0;
    for (i = 0; i < valid_count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= valid_count;
    double std_dev = sqrt(variance);
    
    double approx_pi = 0.0;
    int sign = 1;
    for (int k = 0; k < MAX_ITERATIONS; k++) {
        double term = 1.0 / (2 * k + 1);
        approx_pi += sign * term;
        sign = -sign;
    }
    approx_pi *= 4;
    
    printf("\nResults:\n");
    printf("Numbers processed: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Product: %.6f\n", product);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Pi approximation (Leibniz): %.10f\n", approx_pi);
    
    return 0;
}