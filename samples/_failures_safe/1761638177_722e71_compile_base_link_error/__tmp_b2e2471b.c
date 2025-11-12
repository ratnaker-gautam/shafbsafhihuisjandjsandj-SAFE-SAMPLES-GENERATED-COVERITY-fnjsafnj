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
    if (arr == NULL || size <= 0) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *ptr = (i * 3 + 7) % 23;
        ptr++;
    }
}

void compute_stats(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || size <= 0 || mean == NULL || stddev == NULL) return;
    
    double sum = 0.0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    *mean = sum / size;
    
    double variance = 0.0;
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *ptr - *mean;
        variance += diff * diff;
        ptr++;
    }
    *stddev = sqrt(variance / size);
}

void print_array(int* arr, int size) {
    if (arr == NULL || size <= 0) return;
    
    printf("Array elements: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d", *ptr);
        if (i < size - 1) printf(", ");
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
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    print_array(arr, size);
    
    double mean, stddev;
    compute_stats(arr, size, &mean, &stddev);
    
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    
    int* ptr = arr;
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    printf("Sum: %d\n", sum);
    
    return 0;
}