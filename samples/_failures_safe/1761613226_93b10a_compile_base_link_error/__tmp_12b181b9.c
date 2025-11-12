//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, double* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input) return 0;
    if (*endptr != '\0' && *endptr != '\n') return 0;
    if (*result == HUGE_VAL || *result == -HUGE_VAL) return 0;
    
    return 1;
}

void compute_statistics(double* data, size_t count, double* results) {
    if (data == NULL || results == NULL || count == 0) return;
    
    double* ptr = data;
    double sum = 0.0;
    double min_val = *ptr;
    double max_val = *ptr;
    
    while (ptr < data + count) {
        double val = *ptr;
        sum += val;
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
        ptr++;
    }
    
    double mean = sum / count;
    
    ptr = data;
    double variance_sum = 0.0;
    while (ptr < data + count) {
        double diff = *ptr - mean;
        variance_sum += diff * diff;
        ptr++;
    }
    
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    results[0] = mean;
    results[1] = min_val;
    results[2] = max_val;
    results[3] = std_dev;
}

int main() {
    double data[ARRAY_SIZE];
    double results[4];
    char input_buffer[256];
    size_t count = 0;
    
    printf("Enter up to %d numbers (one per line, empty line to finish):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        
        if (input_buffer[0] == '\n') break;
        
        double value;
        if (!validate_input(input_buffer, &value)) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        data[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    compute_statistics(data, count, results);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", results[0]);
    printf("Minimum: %.6f\n", results[1]);
    printf("Maximum: %.6f\n", results[2]);
    printf("Standard Deviation: %.6f\n", results[3]);
    
    return 0;
}