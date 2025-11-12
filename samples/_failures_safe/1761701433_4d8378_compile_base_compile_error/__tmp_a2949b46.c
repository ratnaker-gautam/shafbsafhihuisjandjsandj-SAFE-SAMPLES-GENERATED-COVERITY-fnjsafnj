//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

double safe_add(double a, double b) {
    if (fabs(a) > DBL_MAX - fabs(b)) return DBL_MAX;
    return a + b;
}

double safe_multiply(double a, double b) {
    if (fabs(b) < 1.0) return a * b;
    if (fabs(a) > DBL_MAX / fabs(b)) return DBL_MAX;
    return a * b;
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (!is_valid_double(input)) {
            printf("Invalid number. Please enter a valid number.\n");
            continue;
        }
        
        double num = strtod(input, NULL);
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nComputing statistics for %d numbers:\n", count);
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum = safe_add(sum, numbers[i]);
        product = safe_multiply(product, numbers[i]);
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance_sum = safe_add(variance_sum, diff * diff);
    }
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nEnter a number to find its square root: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        
        if (is_valid_double(input)) {
            double num = strtod(input, NULL);
            if (num >= 0) {
                printf("Square root: %.6f\n", sqrt(num));
            } else {
                printf("Cannot compute square root of negative number.\n");
            }
        } else {
            printf("Invalid input for square root calculation.\n");
        }
    }
    
    return 0;
}