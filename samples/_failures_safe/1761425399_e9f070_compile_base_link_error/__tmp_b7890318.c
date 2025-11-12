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
        double current = numbers[i];
        sum += current;
        
        if (current < min_val) min_val = current;
        if (current > max_val) max_val = current;
        
        i++;
    }
    
    int j = 0;
    do {
        if (fabs(numbers[j]) < 1e-300) {
            printf("Warning: very small number may cause precision issues\n");
        }
        product *= numbers[j];
        j++;
    } while (j < valid_count);
    
    double mean = sum / valid_count;
    double variance = 0.0;
    
    for (int k = 0; k < valid_count; k++) {
        double diff = numbers[k] - mean;
        variance += diff * diff;
    }
    variance /= valid_count;
    double std_dev = sqrt(variance);
    
    double geometric_mean = pow(fabs(product), 1.0 / valid_count);
    if (product < 0 && valid_count % 2 == 1) {
        geometric_mean = -geometric_mean;
    }
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    printf("\nComputing approximations using iterative methods:\n");
    double approx_pi = 0.0;
    int terms = (valid_count < MAX_ITERATIONS) ? valid_count : MAX_ITERATIONS;
    
    for (int n = 0; n < terms; n++) {
        double term = (n % 2 == 0) ? 1.0 : -1.0;
        term /= (2 * n + 1);
        approx_pi += term;
    }
    approx_pi *= 4;
    
    printf("Pi approximation (Leibniz): %.10f\n", approx_pi);
    
    double approx_e = 1.0;
    double factorial = 1.0;
    int m = 1;
    while (m <= terms && m < 20) {
        factorial *= m;
        if (!isfinite(factorial)) break;
        approx_e += 1.0 / factorial;
        m++;
    }
    
    printf("e approximation: %.10f\n", approx_e);
    
    return 0;
}