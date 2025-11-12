//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < INT_MIN || value > INT_MAX) return -1;
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t count, double* results) {
    if (data == NULL || results == NULL || count == 0) return;
    
    int* ptr = data;
    long long sum = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    
    for (size_t i = 0; i < count; i++) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    results[0] = (double)sum / count;
    results[1] = (double)min;
    results[2] = (double)max;
    
    double variance = 0.0;
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        double diff = *ptr - results[0];
        variance += diff * diff;
        ptr++;
    }
    results[3] = sqrt(variance / count);
}

int main() {
    int numbers[ARRAY_SIZE];
    double stats[4];
    char input_buffer[64];
    size_t count = 0;
    
    printf("Enter up to %d integers (type 'done' to finish):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        printf("Number %zu: ", count + 1);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "done") == 0) {
            break;
        }
        
        int value;
        if (validate_input(input_buffer, &value) == 0) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input. Please enter an integer or 'done'.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    compute_statistics(numbers, count, stats);
    
    printf("\nStatistical Results:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Min: %.2f\n", stats[1]);
    printf("Max: %.2f\n", stats[2]);
    printf("Standard Deviation: %.2f\n", stats[3]);
    
    return 0;
}