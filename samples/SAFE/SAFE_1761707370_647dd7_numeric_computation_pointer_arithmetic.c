//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_integer_input(const char* input, int* result) {
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

void compute_statistics(int* data, size_t count, int* min, int* max, double* mean) {
    if (data == NULL || count == 0) {
        *min = 0;
        *max = 0;
        *mean = 0.0;
        return;
    }
    
    int* ptr = data;
    int local_min = *ptr;
    int local_max = *ptr;
    long long sum = 0;
    
    for (size_t i = 0; i < count; i++) {
        if (*ptr < local_min) local_min = *ptr;
        if (*ptr > local_max) local_max = *ptr;
        sum += *ptr;
        ptr++;
    }
    
    *min = local_min;
    *max = local_max;
    *mean = (double)sum / count;
}

int main() {
    int numbers[ARRAY_SIZE];
    char input_buffer[256];
    size_t count = 0;
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (input_buffer[0] == '\n') {
            break;
        }
        
        int value;
        if (validate_integer_input(input_buffer, &value)) {
            *(numbers + count) = value;
            count++;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    int min_val, max_val;
    double mean_val;
    
    compute_statistics(numbers, count, &min_val, &max_val, &mean_val);
    
    printf("\nStatistics for %zu numbers:\n", count);
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Mean: %.2f\n", mean_val);
    
    int* ptr = numbers;
    printf("\nNumbers squared:\n");
    for (size_t i = 0; i < count; i++) {
        long long square = (long long)(*ptr) * (long long)(*ptr);
        if (square > INT_MAX) {
            printf("Overflow calculating square for %d\n", *ptr);
        } else {
            printf("%lld ", square);
        }
        ptr++;
    }
    printf("\n");
    
    return 0;
}