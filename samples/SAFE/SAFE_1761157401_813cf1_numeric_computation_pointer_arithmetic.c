//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, int* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input) return 0;
    if (*endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

void fill_array(int* arr, size_t size, int start_val) {
    if (arr == NULL) return;
    
    int* end = arr + size;
    int* current = arr;
    int value = start_val;
    
    while (current < end) {
        *current = value;
        value = (value * 3 + 7) % 100;
        current++;
    }
}

double compute_statistics(int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0.0;
    
    int* end = arr + size;
    int* current = arr;
    
    long long sum = 0;
    long long sum_squares = 0;
    size_t count = 0;
    
    while (current < end) {
        sum += *current;
        sum_squares += (long long)(*current) * (long long)(*current);
        count++;
        current++;
    }
    
    if (count == 0) return 0.0;
    
    double mean = (double)sum / count;
    double variance = ((double)sum_squares / count) - (mean * mean);
    
    return sqrt(fabs(variance));
}

int main() {
    char input_buffer[64];
    int start_value;
    
    printf("Enter starting value for array generation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &start_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    int data_array[ARRAY_SIZE];
    fill_array(data_array, ARRAY_SIZE, start_value);
    
    double std_dev = compute_statistics(data_array, ARRAY_SIZE);
    
    printf("Generated array statistics:\n");
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("First 10 elements: ");
    int* ptr = data_array;
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}