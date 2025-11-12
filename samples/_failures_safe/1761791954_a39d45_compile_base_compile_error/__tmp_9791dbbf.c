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
    long long sum = 0;
    int min = *ptr;
    int max = *ptr;
    
    for (size_t i = 0; i < count; i++) {
        if (ptr < data || ptr >= data + count) break;
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
        if (ptr < data || ptr >= data + count) break;
        double diff = *ptr - results[0];
        variance += diff * diff;
        ptr++;
    }
    results[3] = sqrt(variance / count);
}

int main() {
    int numbers[ARRAY_SIZE];
    double stats[4];
    char input_buffer[32];
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", ARRAY_SIZE);
    
    int* current = numbers;
    size_t count = 0;
    
    while (count < ARRAY_SIZE) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        int value;
        if (validate_input(input_buffer, &value) == 0) {
            *current = value;
            current++;
            count++;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    compute_statistics(numbers, count, stats);
    
    printf("\nStatistics for %zu numbers:\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Min: %.2f\n", stats[1]);
    printf("Max: %.2f\n", stats[2]);
    printf("Standard deviation: %.2f\n", stats[3]);
    
    printf("\nNumbers in reverse order:\n");
    int* ptr = numbers + count - 1;
    for (size_t i = 0; i < count; i++) {
        if (ptr < numbers || ptr >= numbers + count) break;
        printf("%d ", *ptr);
        ptr--;
    }
    printf("\n");
    
    return 0;
}