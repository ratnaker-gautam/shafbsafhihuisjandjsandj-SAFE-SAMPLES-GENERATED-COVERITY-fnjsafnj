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
    int* end = data + count;
    
    long long sum = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    
    while (ptr < end) {
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
    while (ptr < end) {
        double diff = *ptr - results[0];
        variance += diff * diff;
        ptr++;
    }
    results[3] = sqrt(variance / count);
}

int main() {
    int numbers[ARRAY_SIZE];
    double stats[4];
    char buffer[64];
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", ARRAY_SIZE);
    
    int* current = numbers;
    int* end = numbers + ARRAY_SIZE;
    int count = 0;
    
    while (current < end) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (buffer[0] == '\n') break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        int value;
        if (validate_input(buffer, &value) == 0) {
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
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Min: %.0f\n", stats[1]);
    printf("Max: %.0f\n", stats[2]);
    printf("Standard Deviation: %.2f\n", stats[3]);
    
    int* ptr = numbers;
    printf("\nSorted values: ");
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (*(numbers + j) > *(numbers + j + 1)) {
                int temp = *(numbers + j);
                *(numbers + j) = *(numbers + j + 1);
                *(numbers + j + 1) = temp;
            }
        }
    }
    
    ptr = numbers;
    while (ptr < numbers + count) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}