//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double calculate_mean(double* data, int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum / count;
}

double calculate_stddev(double* data, int count, double mean) {
    double sum_sq = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / count);
}

int read_double_array(double* array, int max_size) {
    char input[MAX_INPUT_LEN];
    int count = 0;
    
    printf("Enter numbers separated by spaces (max %d): ", max_size);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    char* token = strtok(input, " \t\n");
    while (token != NULL && count < max_size) {
        char* endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            printf("Invalid input: %s\n", token);
            return -1;
        }
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range: %s\n", token);
            return -1;
        }
        array[count++] = value;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int main(void) {
    double numbers[100];
    int count;
    
    printf("Statistical Calculator\n");
    printf("=====================\n");
    
    count = read_double_array(numbers, 100);
    if (count <= 0) {
        if (count == 0) {
            printf("No valid numbers entered.\n");
        }
        return 1;
    }
    
    if (count < 2) {
        printf("At least 2 numbers required for statistics.\n");
        return 1;
    }
    
    double mean = calculate_mean(numbers, count);
    double stddev = calculate_stddev(numbers, count, mean);
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    
    double min_val = numbers[0];
    double max_val = numbers[0];
    for (int i = 1; i < count; i++) {
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
    }
    
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Range: %.6f\n", max_val - min_val);
    
    return 0;
}