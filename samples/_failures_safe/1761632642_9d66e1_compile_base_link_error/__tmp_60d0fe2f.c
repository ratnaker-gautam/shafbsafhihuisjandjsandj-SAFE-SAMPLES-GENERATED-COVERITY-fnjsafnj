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
    for (int i = 0; i < size; i++) {
        *ptr = (i + 1) * (i + 1);
        ptr++;
    }
}

double compute_mean(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return 0.0;
    
    long long sum = 0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    
    return (double)sum / size;
}

double compute_stddev(int* arr, int size, double mean) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return 0.0;
    
    double sum_sq = 0.0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *ptr - mean;
        sum_sq += diff * diff;
        ptr++;
    }
    
    return sqrt(sum_sq / size);
}

void print_results(int* arr, int size, double mean, double stddev) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    printf("Array elements (squares):\n");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
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
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter an integer between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    double mean = compute_mean(arr, size);
    double stddev = compute_stddev(arr, size, mean);
    
    print_results(arr, size, mean, stddev);
    
    return 0;
}