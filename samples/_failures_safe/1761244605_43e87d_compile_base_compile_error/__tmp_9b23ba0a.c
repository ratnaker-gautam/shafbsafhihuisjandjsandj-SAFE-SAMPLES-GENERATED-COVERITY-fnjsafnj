//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char *input, int *valid) {
    if (input == NULL || valid == NULL) return -1;
    
    int len = strlen(input);
    if (len == 0 || len > 10) return -1;
    
    for (int i = 0; i < len; i++) {
        if (i == 0 && input[i] == '-') continue;
        if (input[i] < '0' || input[i] > '9') return -1;
    }
    
    *valid = 1;
    return 0;
}

int parse_int(const char *input, int *result) {
    if (input == NULL || result == NULL) return -1;
    
    int valid = 0;
    if (validate_input(input, &valid) != 0 || valid == 0) return -1;
    
    long long temp = 0;
    int sign = 1;
    const char *ptr = input;
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
    
    while (*ptr != '\0') {
        if (temp > INT_MAX / 10) return -1;
        temp = temp * 10 + (*ptr - '0');
        if (temp < 0) return -1;
        ptr++;
    }
    
    temp *= sign;
    if (temp < INT_MIN || temp > INT_MAX) return -1;
    
    *result = (int)temp;
    return 0;
}

void compute_statistics(int *arr, int size, double *results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int *ptr = arr;
    long long sum = 0;
    long long sum_sq = 0;
    
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
        if (sum < -1000000000LL || sum > 1000000000LL) return;
    }
    
    double mean = (double)sum / size;
    
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *(ptr + i) - mean;
        sum_sq += (long long)(diff * diff);
        if (sum_sq < 0 || sum_sq > 1000000000000LL) return;
    }
    
    double variance = (double)sum_sq / size;
    double std_dev = sqrt(variance);
    
    ptr = arr;
    int min = *ptr;
    int max = *ptr;
    for (int i = 1; i < size; i++) {
        if (*(ptr + i) < min) min = *(ptr + i);
        if (*(ptr + i) > max) max = *(ptr + i);
    }
    
    results[0] = mean;
    results[1] = variance;
    results[2] = std_dev;
    results[3] = (double)min;
    results[4] = (double)max;
}

int main() {
    int numbers[MAX_SIZE];
    int count = 0;
    char buffer[20];
    
    printf("Enter up to %d integers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        int num;
        if (parse_int(buffer, &num) == 0) {
            numbers[count] = num;
            count++;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    double stats[5];
    compute_statistics(numbers, count, stats);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Variance: %.2f\n", stats[1]);
    printf("Standard Deviation: %.2f\n", stats[2]);
    printf("Minimum: %.0f\n", stats[3]);
    printf("Maximum: %.0f\n", stats[4]);
    
    return 0;
}