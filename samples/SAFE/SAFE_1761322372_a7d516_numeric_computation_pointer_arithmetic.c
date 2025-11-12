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
    
    long result = 0;
    int sign = 1;
    const char* ptr = str;
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
    
    while (*ptr != '\0') {
        if (result > (LONG_MAX - (*ptr - '0')) / 10) {
            return 0;
        }
        result = result * 10 + (*ptr - '0');
        ptr++;
    }
    
    result *= sign;
    
    if (result < INT_MIN || result > INT_MAX) {
        return 0;
    }
    
    *valid = (int)result;
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
        if (i > 0 && (sum > LLONG_MAX - *(ptr + i) || sum < LLONG_MIN + *(ptr + i))) {
            return;
        }
        sum += *(ptr + i);
        
        long long square = (long long)*(ptr + i) * *(ptr + i);
        if (sum_sq > LLONG_MAX - square) {
            return;
        }
        sum_sq += square;
        
        if (*(ptr + i) < min_val) {
            min_val = *(ptr + i);
        }
        if (*(ptr + i) > max_val) {
            max_val = *(ptr + i);
        }
    }
    
    double mean = (double)sum / size;
    double variance = ((double)sum_sq / size) - (mean * mean);
    
    if (variance < 0) variance = 0;
    double std_dev = sqrt(variance);
    
    results[0] = mean;
    results[1] = std_dev;
    results[2] = (double)min_val;
    results[3] = (double)max_val;
}

int main() {
    int numbers[MAX_SIZE];
    int count = 0;
    char input[20];
    
    printf("Enter up to %d integers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        printf("Number %d: ", count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (input[0] == '\0') {
            break;
        }
        
        int value;
        if (validate_input(input, &value)) {
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
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard Deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    printf("\nArray elements using pointer arithmetic:\n");
    int* ptr = numbers;
    for (int i = 0; i < count; i++) {
        printf("Element %d: %d\n", i + 1, *(ptr + i));
    }
    
    return 0;
}