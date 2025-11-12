//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    int min = *ptr;
    int max = *ptr;
    long long sum = 0;
    
    for (size_t i = 0; i < count; i++) {
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        sum += *ptr;
        ptr++;
    }
    
    double mean = (double)sum / count;
    double variance = 0.0;
    
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= count;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = (double)min;
    results[3] = (double)max;
}

int main() {
    int numbers[ARRAY_SIZE];
    double stats[4];
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", ARRAY_SIZE);
    
    char buffer[256];
    size_t count = 0;
    
    while (count < ARRAY_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (buffer[0] == '\n') break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        int value;
        if (validate_input(buffer, &value) == 0) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input. Please enter an integer.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    compute_statistics(numbers, count, stats);
    
    printf("\nStatistics for %zu numbers:\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard Deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    return 0;
}