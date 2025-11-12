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

void compute_statistics(int* arr, size_t size, double* results) {
    if (arr == NULL || results == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    long long sum = 0;
    int min = *ptr;
    int max = *ptr;
    
    while (ptr < end) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = (double)sum / size;
    
    ptr = arr;
    double variance_sum = 0.0;
    while (ptr < end) {
        double diff = *ptr - mean;
        variance_sum += diff * diff;
        ptr++;
    }
    double variance = variance_sum / size;
    double std_dev = sqrt(variance);
    
    results[0] = mean;
    results[1] = std_dev;
    results[2] = (double)min;
    results[3] = (double)max;
}

void fill_array(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    int value = 1;
    while (ptr < end) {
        *ptr = value * value - 3 * value + 7;
        value++;
        ptr++;
    }
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    double results[4];
    
    fill_array(arr, size);
    compute_statistics(arr, size, results);
    
    printf("Array values: ");
    int* ptr = arr;
    int* end = arr + size;
    while (ptr < end) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    printf("Mean: %.2f\n", results[0]);
    printf("Standard deviation: %.2f\n", results[1]);
    printf("Minimum: %.0f\n", results[2]);
    printf("Maximum: %.0f\n", results[3]);
    
    return 0;
}