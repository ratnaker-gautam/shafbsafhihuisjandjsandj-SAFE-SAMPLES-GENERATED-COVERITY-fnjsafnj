//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100

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
    char input[MAX_INPUT_LEN + 1];
    char* token;
    int count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        return -1;
    }
    
    token = strtok(input, " ");
    while (token != NULL && count < max_size) {
        char* endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (value < -1e100 || value > 1e100) {
            return -1;
        }
        
        array[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;
}

int main() {
    double numbers[100];
    int count;
    
    printf("Enter up to 100 numbers separated by spaces: ");
    count = read_double_array(numbers, 100);
    
    if (count <= 0) {
        printf("Error: Invalid input or no numbers provided\n");
        return 1;
    }
    
    if (count == 1) {
        printf("Only one number provided: %.6f\n", numbers[0]);
        printf("Mean: %.6f\n", numbers[0]);
        printf("Standard deviation: 0.000000\n");
        return 0;
    }
    
    double mean = calculate_mean(numbers, count);
    double stddev = calculate_stddev(numbers, count, mean);
    
    printf("Statistics for %d numbers:\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    
    return 0;
}