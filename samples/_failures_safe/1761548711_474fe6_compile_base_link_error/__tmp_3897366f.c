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
        int result = scanf("%lf", &input);
        
        if (result != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid input: non-finite number\n");
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
    
    for (int j = 0; j < valid_count; j++) {
        if (numbers[j] != 0.0) {
            if (fabs(log(fabs(product))) + fabs(log(fabs(numbers[j]))) > 300.0) {
                printf("Warning: Product calculation may overflow\n");
                product = 0.0;
                break;
            }
            product *= numbers[j];
        }
    }
    
    double variance = 0.0;
    for (int k = 0; k < valid_count; k++) {
        double diff = numbers[k] - mean;
        variance += diff * diff;
    }
    variance /= valid_count;
    double std_dev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nComputing square roots (iterative method):\n");
    for (int idx = 0; idx < valid_count; idx++) {
        double x = fabs(numbers[idx]);
        double guess = x / 2.0;
        int iterations = 0;
        
        while (iterations < MAX_ITERATIONS) {
            double new_guess = 0.5 * (guess + x / guess);
            if (fabs(new_guess - guess) < 1e-10) {
                break;
            }
            guess = new_guess;
            iterations++;
        }
        
        printf("Number: %.6f, Square root: %.6f (iterations: %d)\n", 
               numbers[idx], numbers[idx] >= 0 ? guess : -guess, iterations);
    }
    
    return 0;
}