//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    int len = strlen(input);
    if (len == 0 || len > 10) return -1;
    
    for (int i = 0; i < len; i++) {
        if (i == 0 && input[i] == '-') continue;
        if (input[i] < '0' || input[i] > '9') return -1;
    }
    
    long temp = 0;
    int sign = 1;
    int start = 0;
    
    if (input[0] == '-') {
        sign = -1;
        start = 1;
    }
    
    for (int i = start; i < len; i++) {
        if (temp > (LONG_MAX - (input[i] - '0')) / 10) return -1;
        temp = temp * 10 + (input[i] - '0');
    }
    
    temp *= sign;
    if (temp < INT_MIN || temp > INT_MAX) return -1;
    
    *valid = (int)temp;
    return 0;
}

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int sum = 0;
    int min = arr[0];
    int max = arr[0];
    
    for (int i = 0; i < size; i++) {
        sum += *(arr + i);
        if (*(arr + i) < min) min = *(arr + i);
        if (*(arr + i) > max) max = *(arr + i);
    }
    
    double mean = (double)sum / size;
    double variance = 0.0;
    
    for (int i = 0; i < size; i++) {
        double diff = *(arr + i) - mean;
        variance += diff * diff;
    }
    variance /= size;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = (double)min;
    results[3] = (double)max;
}

int main() {
    int numbers[MAX_SIZE];
    int count = 0;
    char buffer[20];
    
    printf("Enter up to %d integers (empty line to stop):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        int value;
        if (validate_input(buffer, &value) == 0) {
            *(numbers + count) = value;
            count++;
        } else {
            printf("Invalid input: %s\n", buffer);
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double stats[4];
    compute_statistics(numbers, count, stats);
    
    printf("\nResults for %d numbers:\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    printf("\nArray elements using pointer arithmetic:\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", *(numbers + i));
    }
    printf("\n");
    
    return 0;
}