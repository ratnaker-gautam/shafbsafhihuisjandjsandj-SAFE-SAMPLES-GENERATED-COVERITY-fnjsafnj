//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
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
    const char* ptr = input;
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
    
    while (*ptr != '\0') {
        if (temp > (LONG_MAX - (*ptr - '0')) / 10) return -1;
        temp = temp * 10 + (*ptr - '0');
        ptr++;
    }
    
    temp *= sign;
    if (temp < INT_MIN || temp > INT_MAX) return -1;
    
    *valid = (int)temp;
    return 0;
}

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int* ptr = arr;
    long long sum = 0;
    int min = *ptr;
    int max = *ptr;
    
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
        if (*(ptr + i) < min) min = *(ptr + i);
        if (*(ptr + i) > max) max = *(ptr + i);
    }
    
    results[0] = (double)sum / size;
    results[1] = (double)min;
    results[2] = (double)max;
    
    double variance = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = *(ptr + i) - results[0];
        variance += diff * diff;
    }
    results[3] = sqrt(variance / size);
}

int main() {
    int numbers[MAX_SIZE];
    int count = 0;
    char buffer[64];
    
    printf("Enter up to %d integers (empty line to finish):\n", MAX_SIZE);
    
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
            numbers[count] = value;
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
    printf("Min: %.2f\n", stats[1]);
    printf("Max: %.2f\n", stats[2]);
    printf("Std Dev: %.2f\n", stats[3]);
    
    return 0;
}