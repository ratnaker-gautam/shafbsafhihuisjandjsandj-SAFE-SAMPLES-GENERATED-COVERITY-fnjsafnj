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
    
    double harmonic_sum = 0.0;
    for (int m = 0; m < valid_count; m++) {
        if (numbers[m] != 0.0) {
            harmonic_sum += 1.0 / numbers[m];
        } else {
            harmonic_sum = INFINITY;
            break;
        }
    }
    
    double harmonic_mean = (harmonic_sum != 0.0 && isfinite(harmonic_sum)) ? 
                          valid_count / harmonic_sum : 0.0;
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    printf("\nComputing approximations of pi using different methods:\n");
    
    double pi_approx1 = 0.0;
    for (int n = 0; n < MAX_ITERATIONS; n++) {
        pi_approx1 += (n % 2 == 0 ? 1.0 : -1.0) / (2.0 * n + 1.0);
    }
    pi_approx1 *= 4.0;
    
    double pi_approx2 = 0.0;
    int n = 0;
    while (n < MAX_ITERATIONS) {
        pi_approx2 += 1.0 / ((4.0 * n + 1.0) * (4.0 * n + 3.0));
        n++;
    }
    pi_approx2 *= 8.0;
    
    double pi_approx3 = 0.0;
    n = 1;
    do {
        pi_approx3 += 1.0 / (n * n);
        n++;
    } while (n <= MAX_ITERATIONS);
    pi_approx3 = sqrt(pi_approx3 * 6.0);
    
    printf("Leibniz series: %.10f\n", pi_approx1);
    printf("Nilakantha-like: %.10f\n", pi_approx2);
    printf("Basel problem: %.10f\n", pi_approx3);
    
    return 0;
}