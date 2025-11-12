//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

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
    
    long temp = 0;
    int sign = 1;
    const char* p = str;
    
    if (*p == '-') {
        sign = -1;
        p++;
    }
    
    while (*p != '\0') {
        if (temp > (LONG_MAX - (*p - '0')) / 10) {
            return 0;
        }
        temp = temp * 10 + (*p - '0');
        p++;
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
    
    int* p = arr;
    long long sum = 0;
    long long sum_sq = 0;
    
    for (int i = 0; i < size; i++) {
        if ((*p > 0 && sum > LLONG_MAX - *p) || (*p < 0 && sum < LLONG_MIN - *p)) return;
        sum += *(p + i);
    }
    
    double mean = (double)sum / size;
    
    p = arr;
    for (int i = 0; i < size; i++) {
        double diff = *(p + i) - mean;
        long long diff_sq = (long long)(diff * diff);
        if (sum_sq > LLONG_MAX - diff_sq) return;
        sum_sq += diff_sq;
    }
    
    double variance = (double)sum_sq / size;
    double std_dev = sqrt(variance);
    
    p = arr;
    int min = *p;
    int max = *p;
    
    for (int i = 1; i < size; i++) {
        if (*(p + i) < min) min = *(p + i);
        if (*(p + i) > max) max = *(p + i);
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
    char input[20];
    
    printf("Enter up to %d integers (empty line to finish):\n", MAX_SIZE);
    
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
            printf("Invalid input. Please enter an integer between %d and %d.\n", INT_MIN, INT_MAX);
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
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
    
    printf("\nArray elements using pointer arithmetic:\n");
    int* ptr = numbers;
    for (int i = 0; i < count; i++) {
        printf("%d ", *(ptr + i));
        if ((i + 1) % 10 == 0) printf("\n");
    }
    if (count % 10 != 0) printf("\n");
    
    return 0;
}