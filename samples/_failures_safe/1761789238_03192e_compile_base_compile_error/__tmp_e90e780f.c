//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

int safe_str_to_int(const char* str) {
    long result = 0;
    const char* ptr = str;
    while (*ptr != '\0') {
        if (result > (LONG_MAX - (*ptr - '0')) / 10) {
            return -1;
        }
        result = result * 10 + (*ptr - '0');
        ptr++;
    }
    if (result > INT_MAX) return -1;
    return (int)result;
}

void compute_statistics(int* arr, size_t size, double* results) {
    if (arr == NULL || results == NULL || size == 0) return;
    
    int* ptr = arr;
    int min = *ptr;
    int max = *ptr;
    long long sum = 0;
    
    for (size_t i = 0; i < size; i++) {
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        if (sum > LLONG_MAX - *ptr) {
            results[0] = 0.0;
            results[1] = 0.0;
            results[2] = 0.0;
            results[3] = 0.0;
            return;
        }
        sum += *ptr;
        ptr++;
    }
    
    double mean = (double)sum / size;
    double variance = 0.0;
    
    ptr = arr;
    for (size_t i = 0; i < size; i++) {
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
    size_t count = 0;
    char buffer[12];
    
    printf("Enter up to %d integers (empty line to stop):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (!validate_input(buffer)) {
            printf("Invalid input. Please enter positive integers only.\n");
            continue;
        }
        
        int num = safe_str_to_int(buffer);
        if (num == -1) {
            printf("Number too large. Please enter smaller values.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double stats[4];
    compute_statistics(numbers, count, stats);
    
    printf("\nResults for %zu numbers:\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    return 0;
}