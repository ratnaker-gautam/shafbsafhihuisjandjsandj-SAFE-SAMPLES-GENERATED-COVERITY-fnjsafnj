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
            if (i == 0 && str[i] == '-') continue;
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
    int min = *ptr;
    int max = *ptr;
    
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
        if (*(ptr + i) < min) min = *(ptr + i);
        if (*(ptr + i) > max) max = *(ptr + i);
    }
    
    double mean = sum / size;
    double variance = 0.0;
    
    for (int i = 0; i < size; i++) {
        double diff = *(ptr + i) - mean;
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
    char input[20];
    
    printf("Enter up to %d integers (empty line to stop):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        int value;
        if (validate_input(input, &value)) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input. Please enter an integer between -%d and %d.\n", MAX_SIZE, MAX_SIZE);
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    double stats[4];
    compute_statistics(numbers, count, stats);
    
    printf("\nStatistics for %d numbers:\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    return 0;
}