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
        *ptr = (i * 2 + 1) * (i % 3 + 1);
        ptr++;
    }
}

double compute_stats(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || mean == NULL || stddev == NULL || size < 1) return -1;
    
    long long sum = 0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    
    *mean = (double)sum / size;
    
    double variance = 0.0;
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *ptr - *mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= size;
    *stddev = sqrt(variance);
    
    return 0;
}

int find_median(int* arr, int size) {
    if (arr == NULL || size < 1) return -1;
    
    int* temp = malloc(sizeof(int) * size);
    if (temp == NULL) return -1;
    
    memcpy(temp, arr, sizeof(int) * size);
    
    for (int i = 0; i < size - 1; i++) {
        int* min_ptr = temp + i;
        for (int* j = temp + i + 1; j < temp + size; j++) {
            if (*j < *min_ptr) {
                min_ptr = j;
            }
        }
        if (min_ptr != temp + i) {
            int swap = *min_ptr;
            *min_ptr = *(temp + i);
            *(temp + i) = swap;
        }
    }
    
    int median;
    if (size % 2 == 0) {
        median = (*(temp + size/2 - 1) + *(temp + size/2)) / 2;
    } else {
        median = *(temp + size/2);
    }
    
    free(temp);
    return median;
}

int main() {
    char input[32];
    printf("Enter array size (1-%d): ", MAX_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int size;
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    printf("Generated array: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double mean, stddev;
    if (compute_stats(arr, size, &mean, &stddev) == 0) {
        printf("Mean: %.2f\n", mean);
        printf("Standard deviation: %.2f\n", stddev);
    }
    
    int median = find_median(arr, size);
    if (median != -1) {
        printf("Median: %d\n", median);
    }
    
    return 0;
}