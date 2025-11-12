//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* str, int* valid) {
    if (str == NULL || valid == NULL) return 0;
    
    int len = strlen(str);
    if (len == 0 || len > 10) return 0;
    
    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            if (i == 0 && str[i] == '-') continue;
            return 0;
        }
    }
    
    long temp = 0;
    int sign = 1;
    const char* ptr = str;
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
    
    while (*ptr != '\0') {
        if (temp > (LONG_MAX - (*ptr - '0')) / 10) {
            return 0;
        }
        temp = temp * 10 + (*ptr - '0');
        ptr++;
    }
    
    temp *= sign;
    
    if (temp < INT_MIN || temp > INT_MAX) {
        return 0;
    }
    
    *valid = (int)temp;
    return 1;
}

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    double sum = 0.0;
    int min = *ptr;
    int max = *ptr;
    
    while (ptr < end) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = sum / size;
    
    ptr = arr;
    double variance = 0.0;
    while (ptr < end) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
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
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", MAX_SIZE);
    
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
            printf("Invalid input: %s\n", input);
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
    printf("Standard Deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    return 0;
}