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
        if (scanf("%lf", &input) != 1) {
            printf("Invalid input. Please enter numeric values only.\n");
            return 1;
        }
        
        if (input < -1000.0 || input > 1000.0) {
            printf("Value out of range (-1000 to 1000).\n");
            return 1;
        }
        
        results[i] = input;
        
        if (fabs(input) > 1e-10) {
            sum += input;
            if (fabs(product) < 1e300) {
                product *= input;
            }
            valid_count++;
        }
    }
    
    double geometric_mean;
    if (valid_count > 0 && product >= 0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    } else {
        geometric_mean = 0.0;
    }
    
    double arithmetic_mean = sum / valid_count;
    
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double variance_sum = 0.0;
    int j = 0;
    while (j < ARRAY_SIZE) {
        if (fabs(results[j]) > 1e-10) {
            double diff = results[j] - arithmetic_mean;
            variance_sum += diff * diff;
        }
        j++;
    }
    
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    int k = 0;
    double harmonic_sum = 0.0;
    do {
        if (fabs(results[k]) > 1e-10) {
            harmonic_sum += 1.0 / results[k];
        }
        k++;
    } while (k < ARRAY_SIZE);
    
    double harmonic_mean;
    if (fabs(harmonic_sum) > 1e-10) {
        harmonic_mean = valid_count / harmonic_sum;
    } else {
        harmonic_mean = 0.0;
    }
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double min_val = results[0];
    double max_val = results[0];
    int m = 1;
    for (; m < ARRAY_SIZE; m++) {
        if (results[m] < min_val) {
            min_val = results[m];
        }
        if (results[m] > max_val) {
            max_val = results[m];
        }
    }
    
    printf("Minimum value: %.6f\n", min_val);
    printf("Maximum value: %.6f\n", max_val);
    
    return 0;
}