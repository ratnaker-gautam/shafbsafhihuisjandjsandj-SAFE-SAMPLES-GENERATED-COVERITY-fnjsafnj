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

int is_valid_number(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    
    int dot_count = 0;
    int digit_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str) {
        if (*str == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (isdigit((unsigned char)*str)) {
            digit_count++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter up to 100 numbers (one per line). Enter 'done' to finish:\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (!is_valid_number(input)) {
            printf("Invalid number. Please enter a valid numeric value.\n");
            continue;
        }
        
        char* endptr;
        double value = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Invalid number format.\n");
            continue;
        }
        
        if (value < -1e100 || value > 1e100) {
            printf("Number out of reasonable range.\n");
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
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    
    printf("\nData points:\n");
    for (int i = 0; i < count; i++) {
        printf("%.6f", numbers[i]);
        if (i < count - 1) {
            printf(", ");
        }
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    return 0;
}