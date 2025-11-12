//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double calculate_mean(const double *data, size_t count) {
    if (count == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum / (double)count;
}

double calculate_stddev(const double *data, size_t count, double mean) {
    if (count <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < count; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (double)(count - 1));
}

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    int has_digit = 0;
    int has_dot = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '-' || str[i] == '+') {
            if (i != 0) return 0;
        } else if (str[i] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else if (isdigit((unsigned char)str[i])) {
            has_digit = 1;
        } else {
            return 0;
        }
    }
    return has_digit;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    size_t count = 0;
    
    printf("Enter up to 100 numbers (one per line). Enter 'done' to finish:\n");
    
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
            continue;
        }
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (!is_valid_number(input)) {
            printf("Invalid number. Please enter a valid number or 'done':\n");
            continue;
        }
        
        char *endptr;
        double value = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Invalid number format. Please enter a valid number or 'done':\n");
            continue;
        }
        
        if (!isfinite(value)) {
            printf("Number out of range. Please enter a finite number or 'done':\n");
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
    
    printf("\nData points:\n");
    for (size_t i = 0; i < count; i++) {
        printf("[%zu]: %.6f", i, numbers[i]);
        if (i < count - 1) printf(", ");
        if ((i + 1) % 5 == 0) printf("\n");
    }
    if (count % 5 != 0) printf("\n");
    
    return 0;
}