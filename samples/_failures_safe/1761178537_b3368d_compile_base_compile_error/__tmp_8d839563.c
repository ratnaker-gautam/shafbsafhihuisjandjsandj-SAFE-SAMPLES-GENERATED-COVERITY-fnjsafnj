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

void process_array(int* arr, size_t size, int factor) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        if (factor != 0) {
            if (*current > 0 && factor > INT_MAX / *current) {
                *current = INT_MAX;
            } else if (*current < 0 && factor < INT_MIN / *current) {
                *current = INT_MIN;
            } else {
                *current *= factor;
            }
        } else {
            *current = 0;
        }
        current++;
    }
}

double calculate_stats(int* arr, size_t size, double* mean, double* stddev) {
    if (arr == NULL || size == 0 || mean == NULL || stddev == NULL) return 0.0;
    
    long long sum = 0;
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        sum += *current;
        current++;
    }
    
    *mean = (double)sum / size;
    
    double variance = 0.0;
    current = arr;
    
    while (current < end) {
        double diff = *current - *mean;
        variance += diff * diff;
        current++;
    }
    
    variance /= size;
    *stddev = sqrt(variance);
    
    return *mean;
}

int main() {
    int array[ARRAY_SIZE];
    int* arr_ptr = array;
    int* end_ptr = array + ARRAY_SIZE;
    
    for (int* ptr = arr_ptr; ptr < end_ptr; ptr++) {
        *ptr = (int)(ptr - arr_ptr) * 2 + 1;
    }
    
    printf("Enter multiplication factor: ");
    char input_buffer[256];
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    int factor;
    if (validate_input(input_buffer, &factor) != 0) {
        fprintf(stderr, "Invalid input: must be an integer\n");
        return 1;
    }
    
    process_array(array, ARRAY_SIZE, factor);
    
    double mean, stddev;
    calculate_stats(array, ARRAY_SIZE, &mean, &stddev);
    
    printf("Array processed with factor %d\n", factor);
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    
    printf("First 10 elements: ");
    int* display_ptr = array;
    for (int i = 0; i < 10 && display_ptr < end_ptr; i++, display_ptr++) {
        printf("%d ", *display_ptr);
    }
    printf("\n");
    
    return 0;
}