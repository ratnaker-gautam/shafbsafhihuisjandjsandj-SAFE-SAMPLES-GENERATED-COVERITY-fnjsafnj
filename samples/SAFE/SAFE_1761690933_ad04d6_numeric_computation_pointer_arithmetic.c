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
    if (val > INT_MAX || val < INT_MIN) return 0;
    
    *result = (int)val;
    return 1;
}

void fill_array(int* arr, size_t size, int start) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    int value = start;
    
    while (current < end) {
        if (value > 0 && value > INT_MAX / 2) value = 1;
        *current = value * value;
        value++;
        current++;
    }
}

double compute_statistics(int* arr, size_t size, double* mean, double* stddev) {
    if (arr == NULL || size == 0 || mean == NULL || stddev == NULL) return 0.0;
    
    int* end = arr + size;
    int* current = arr;
    long long sum = 0;
    long long sum_squares = 0;
    size_t count = 0;
    
    while (current < end) {
        sum += *current;
        if (llabs(sum) > LLONG_MAX / 2) break;
        
        long long square = (long long)(*current) * (long long)(*current);
        sum_squares += square;
        if (llabs(sum_squares) > LLONG_MAX / 2) break;
        
        current++;
        count++;
    }
    
    if (count == 0) return 0.0;
    
    *mean = (double)sum / count;
    
    double variance = ((double)sum_squares / count) - (*mean * *mean);
    *stddev = variance > 0.0 ? sqrt(variance) : 0.0;
    
    return *mean;
}

int find_median(int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0;
    
    int* temp = malloc(size * sizeof(int));
    if (temp == NULL) return 0;
    
    int* src = arr;
    int* dst = temp;
    int* end = arr + size;
    
    while (src < end) {
        *dst = *src;
        src++;
        dst++;
    }
    
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }
    
    int median;
    if (size % 2 == 0) {
        median = (temp[size / 2 - 1] + temp[size / 2]) / 2;
    } else {
        median = temp[size / 2];
    }
    
    free(temp);
    return median;
}

int main() {
    char input_buffer[64];
    int start_value;
    
    printf("Enter starting integer value: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &start_value)) {
        printf("Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    int numbers[ARRAY_SIZE];
    fill_array(numbers, ARRAY_SIZE, start_value);
    
    double mean, stddev;
    compute_statistics(numbers, ARRAY_SIZE, &mean, &stddev);
    int median = find_median(numbers, ARRAY_SIZE);
    
    printf("Array statistics:\n");
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    printf("Median: %d\n", median);
    
    printf("First 10 elements: ");
    int* ptr = numbers;
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}