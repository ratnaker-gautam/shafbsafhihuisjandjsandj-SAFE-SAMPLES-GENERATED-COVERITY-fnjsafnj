//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main(void) {
    double results[ARRAY_SIZE];
    int valid_count = 0;
    double input_value;
    char input_buffer[256];
    char *endptr;
    
    printf("Enter a numeric value between 0.1 and 100.0: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_value = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid numeric input\n");
        return 1;
    }
    
    if (input_value <= 0.0 || input_value > 100.0) {
        fprintf(stderr, "Value out of range (0.1-100.0)\n");
        return 1;
    }
    
    for (int i = 0; i < ARRAY_SIZE && i < MAX_ITERATIONS; i++) {
        double temp = input_value;
        int j = 0;
        
        while (j < 10) {
            temp = sqrt(temp) * 1.5;
            if (temp > 1e100 || temp < -1e100) {
                break;
            }
            j++;
        }
        
        if (j == 10) {
            results[valid_count] = temp;
            valid_count++;
        }
    }
    
    int k = 0;
    do {
        if (k >= valid_count) {
            break;
        }
        
        double sum = 0.0;
        int m = 0;
        while (m <= k) {
            sum += results[m];
            if (m == INT_MAX) {
                break;
            }
            m++;
        }
        
        results[k] = sum / (k + 1);
        k++;
    } while (k < valid_count);
    
    double final_result = 0.0;
    for (int n = 0; n < valid_count; n++) {
        final_result += results[n];
    }
    
    if (valid_count > 0) {
        final_result /= valid_count;
        printf("Computed result: %.6f\n", final_result);
    } else {
        printf("No valid computations performed\n");
    }
    
    return 0;
}