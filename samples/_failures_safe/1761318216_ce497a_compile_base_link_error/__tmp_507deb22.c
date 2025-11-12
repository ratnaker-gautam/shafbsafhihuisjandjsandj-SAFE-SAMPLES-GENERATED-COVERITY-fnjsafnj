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

void fill_array(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        *ptr = (int)(ptr - arr) + 1;
        ptr++;
    }
}

double compute_mean(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return 0.0;
    
    long long sum = 0;
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        sum += *ptr;
        ptr++;
    }
    
    return (double)sum / size;
}

double compute_variance(int* arr, int size, double mean) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return 0.0;
    
    double sum_sq = 0.0;
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        double diff = (double)*ptr - mean;
        sum_sq += diff * diff;
        ptr++;
    }
    
    return sum_sq / size;
}

void print_array(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    printf("Array elements: ");
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        printf("%d", *ptr);
        if (ptr < end - 1) printf(", ");
        ptr++;
    }
    printf("\n");
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
    print_array(arr, size);
    
    double mean = compute_mean(arr, size);
    double variance = compute_variance(arr, size, mean);
    double std_dev = sqrt(variance);
    
    printf("Mean: %.2f\n", mean);
    printf("Variance: %.2f\n", variance);
    printf("Standard deviation: %.2f\n", std_dev);
    
    return 0;
}