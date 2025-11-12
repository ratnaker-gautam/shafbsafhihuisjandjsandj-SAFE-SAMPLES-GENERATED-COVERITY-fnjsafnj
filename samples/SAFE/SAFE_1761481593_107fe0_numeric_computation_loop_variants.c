//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main(void) {
    double results[ARRAY_SIZE];
    int valid_count = 0;
    double input_value;
    char input_buffer[256];
    
    printf("Enter a positive number between 0.1 and 100.0: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    input_value = strtod(input_buffer, &endptr);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input_value <= 0.0 || input_value > 100.0) {
        fprintf(stderr, "Value out of range\n");
        return 1;
    }
    
    double current = input_value;
    int i = 0;
    
    while (i < MAX_ITERATIONS && i < ARRAY_SIZE) {
        if (current < 1e-15) {
            break;
        }
        
        results[i] = sqrt(current);
        valid_count++;
        
        if (fabs(current - 1.0) < 1e-10) {
            break;
        }
        
        current = (current + 1.0/current) / 2.0;
        i++;
    }
    
    printf("Computed %d square root approximations:\n", valid_count);
    
    for (int j = 0; j < valid_count; j++) {
        printf("Iteration %d: %.10f\n", j + 1, results[j]);
    }
    
    double sum = 0.0;
    int k = 0;
    
    do {
        if (k >= valid_count) {
            break;
        }
        sum += results[k];
        k++;
    } while (k < valid_count);
    
    if (valid_count > 0) {
        double average = sum / valid_count;
        printf("Average of computed values: %.10f\n", average);
    }
    
    double product = 1.0;
    
    for (int m = 0; m < valid_count; m++) {
        if (results[m] > 0.0) {
            if (product > DBL_MAX / results[m]) {
                fprintf(stderr, "Product overflow detected\n");
                product = DBL_MAX;
                break;
            }
            product *= results[m];
        }
    }
    
    printf("Product of computed values: %.10f\n", product);
    
    return 0;
}