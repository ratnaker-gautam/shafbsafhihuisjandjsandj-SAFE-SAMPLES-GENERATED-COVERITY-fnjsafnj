//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* str, int* valid) {
    if (str == NULL) return 0;
    
    int len = 0;
    while (str[len] != '\0') {
        if (str[len] < '0' || str[len] > '9') {
            *valid = 0;
            return 0;
        }
        len++;
        if (len >= MAX_SIZE) {
            *valid = 0;
            return 0;
        }
    }
    
    *valid = 1;
    return len;
}

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int* ptr = arr;
    long long sum = 0;
    long long sum_squares = 0;
    
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
        sum_squares += (long long)(*(ptr + i)) * (long long)(*(ptr + i));
    }
    
    results[0] = (double)sum / size;
    
    double variance = ((double)sum_squares / size) - (results[0] * results[0]);
    results[1] = sqrt(variance);
    
    int min_val = *arr;
    int max_val = *arr;
    ptr = arr + 1;
    
    for (int i = 1; i < size; i++) {
        if (*(ptr) < min_val) min_val = *(ptr);
        if (*(ptr) > max_val) max_val = *(ptr);
        ptr++;
    }
    
    results[2] = min_val;
    results[3] = max_val;
}

int main() {
    char input[MAX_SIZE + 1];
    int numbers[MAX_SIZE];
    double stats[4];
    
    printf("Enter up to %d numbers separated by spaces: ", MAX_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* token = strtok(input, " \t\n");
    int count = 0;
    
    while (token != NULL && count < MAX_SIZE) {
        int valid = 0;
        int len = validate_input(token, &valid);
        
        if (!valid || len == 0) {
            printf("Invalid input: %s\n", token);
            return 1;
        }
        
        long value = 0;
        const char* ptr = token;
        
        for (int i = 0; i < len; i++) {
            if (value > (LONG_MAX - (*ptr - '0')) / 10) {
                printf("Number too large: %s\n", token);
                return 1;
            }
            value = value * 10 + (*ptr - '0');
            ptr++;
        }
        
        if (value > INT_MAX) {
            printf("Number too large: %s\n", token);
            return 1;
        }
        
        numbers[count] = (int)value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    if (count == 0) {
        printf("No valid numbers entered\n");
        return 1;
    }
    
    compute_statistics(numbers, count, stats);
    
    printf("Results for %d numbers:\n", count);
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    return 0;
}