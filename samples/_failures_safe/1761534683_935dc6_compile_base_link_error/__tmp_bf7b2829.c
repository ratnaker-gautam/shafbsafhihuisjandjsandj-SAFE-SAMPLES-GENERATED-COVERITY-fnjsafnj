//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    int valid_count = 0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char *endptr;
        double value = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\n') {
            if (*endptr != '\0' && *endptr != '\n') {
                break;
            }
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL || value != value) {
            break;
        }
        
        results[valid_count] = value;
        valid_count++;
        
        if (valid_count >= ARRAY_SIZE) {
            break;
        }
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
            if (fabs(log(fabs(results[j]))) > log(1e308) - log(fabs(temp_product))) {
                temp_product = (temp_product > 0) ? HUGE_VAL : -HUGE_VAL;
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
    
    double min_val = results[0];
    double max_val = results[0];
    int m = 1;
    while (m < valid_count) {
        if (results[m] < min_val) min_val = results[m];
        if (results[m] > max_val) max_val = results[m];
        m++;
    }
    
    printf("Statistical Analysis:\n");
    printf("Count: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nComputing approximations using different methods:\n");
    
    double approx_sum = 0.0;
    for (int n = 0; n < valid_count; n++) {
        approx_sum += sin(results[n]) * cos(results[n]);
    }
    printf("Trigonometric sum approximation: %.6f\n", approx_sum);
    
    double root_sum = 0.0;
    int p = 0;
    while (p < valid_count) {
        if (results[p] >= 0) {
            root_sum += sqrt(results[p]);
        } else {
            root_sum += sqrt(-results[p]);
        }
        p++;
    }
    printf("Root sum approximation: %.6f\n", root_sum);
    
    double log_sum = 0.0;
    int q = 0;
    do {
        if (results[q] > 0) {
            log_sum += log(results[q]);
        }
        q++;
    } while (q < valid_count);
    printf("Logarithmic sum: %.6f\n", log_sum);
    
    return 0;
}