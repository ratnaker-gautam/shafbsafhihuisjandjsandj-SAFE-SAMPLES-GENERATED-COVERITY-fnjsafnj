//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    
    *valid = (int)value;
    return 0;
}

void fill_array(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    int value = 1;
    
    while (current < end) {
        *current = value * value;
        value++;
        current++;
    }
}

double compute_mean(int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0.0;
    
    long long sum = 0;
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        sum += *current;
        current++;
    }
    
    return (double)sum / (double)size;
}

double compute_stddev(int* arr, size_t size, double mean) {
    if (arr == NULL || size == 0) return 0.0;
    
    double sum_sq = 0.0;
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        double diff = (double)*current - mean;
        sum_sq += diff * diff;
        current++;
    }
    
    return sqrt(sum_sq / (double)size);
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter an integer between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    double mean = compute_mean(arr, size);
    double stddev = compute_stddev(arr, size, mean);
    
    printf("Array elements (squares):\n");
    int* end = arr + size;
    int* current = arr;
    while (current < end) {
        printf("%d ", *current);
        current++;
    }
    printf("\n");
    
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    
    return 0;
}