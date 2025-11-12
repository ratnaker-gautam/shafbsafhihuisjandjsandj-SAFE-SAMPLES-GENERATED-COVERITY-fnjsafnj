//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* str, int* valid) {
    if (str == NULL) return 0;
    
    int len = strlen(str);
    if (len == 0 || len > 10) return 0;
    
    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            if (i == 0 && str[i] == '-' && len > 1) continue;
            return 0;
        }
    }
    
    long temp = atol(str);
    if (temp < -MAX_SIZE || temp > MAX_SIZE) return 0;
    
    *valid = (int)temp;
    return 1;
}

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int* ptr = arr;
    double sum = 0.0;
    double sum_sq = 0.0;
    int min = *ptr;
    int max = *ptr;
    
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
        sum_sq += (double)(*(ptr + i)) * (double)(*(ptr + i));
        if (*(ptr + i) < min) min = *(ptr + i);
        if (*(ptr + i) > max) max = *(ptr + i);
    }
    
    double mean = sum / size;
    double variance = (sum_sq / size) - (mean * mean);
    if (variance < 0) variance = 0;
    double std_dev = sqrt(variance);
    
    results[0] = mean;
    results[1] = variance;
    results[2] = std_dev;
    results[3] = (double)min;
    results[4] = (double)max;
}

int main() {
    int size;
    char input[20];
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!validate_input(input, &size) || size < 1 || size > MAX_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    printf("Enter %d integers:\n", size);
    
    for (int i = 0; i < size; i++) {
        printf("Element %d: ", i + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        int value;
        if (!validate_input(input, &value)) {
            printf("Invalid number\n");
            return 1;
        }
        
        *(arr + i) = value;
    }
    
    double results[5];
    compute_statistics(arr, size, results);
    
    printf("\nStatistical Results:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Variance: %.2f\n", results[1]);
    printf("Standard Deviation: %.2f\n", results[2]);
    printf("Minimum: %.0f\n", results[3]);
    printf("Maximum: %.0f\n", results[4]);
    
    return 0;
}