//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double calculate_mean(double *data, size_t count) {
    double sum = 0.0;
    for (size_t i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum / (double)count;
}

double calculate_stddev(double *data, size_t count, double mean) {
    double sum_sq = 0.0;
    for (size_t i = 0; i < count; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (double)count);
}

int parse_double(const char *str, double *result) {
    char *endptr;
    *result = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    size_t count = 0;
    
    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        int valid = 1;
        for (size_t i = 0; i < len; i++) {
            if (!isdigit((unsigned char)input[i]) && input[i] != '.' && 
                input[i] != '-' && input[i] != '+') {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        double value;
        if (!parse_double(input, &value)) {
            printf("Invalid number format. Please try again.\n");
            continue;
        }
        
        if (!isfinite(value)) {
            printf("Number is not finite. Please enter a valid number.\n");
            continue;
        }
        
        numbers[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    double mean = calculate_mean(numbers, count);
    double stddev = calculate_stddev(numbers, count, mean);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    
    printf("\nData points more than 2 standard deviations from mean:\n");
    int found_outliers = 0;
    for (size_t i = 0; i < count; i++) {
        double z_score = fabs((numbers[i] - mean) / stddev);
        if (z_score > 2.0) {
            printf("Index %zu: %.6f (z-score: %.6f)\n", i, numbers[i], z_score);
            found_outliers = 1;
        }
    }
    
    if (!found_outliers) {
        printf("No outliers found.\n");
    }
    
    return 0;
}