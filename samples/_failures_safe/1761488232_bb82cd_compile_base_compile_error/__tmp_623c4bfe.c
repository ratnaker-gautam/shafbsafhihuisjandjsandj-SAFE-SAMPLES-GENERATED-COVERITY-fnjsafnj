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
    long long sum = 0;
    long long sum_sq = 0;
    int min_val = *ptr;
    int max_val = *ptr;
    
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
        long long val = *(ptr + i);
        sum_sq += val * val;
        
        if (*(ptr + i) < min_val) min_val = *(ptr + i);
        if (*(ptr + i) > max_val) max_val = *(ptr + i);
    }
    
    results[0] = (double)sum / size;
    if (size > 1) {
        double variance = ((double)sum_sq - (double)(sum * sum) / size) / (size - 1);
        results[1] = sqrt(variance);
    } else {
        results[1] = 0.0;
    }
    results[2] = (double)min_val;
    results[3] = (double)max_val;
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
        
        int value;
        if (validate_input(buffer, &value)) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
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
    int* ptr = numbers;
    for (int i = 0; i < count; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    return 0;
}