//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

double safe_add(double a, double b) {
    if (fabs(a) > DBL_MAX - fabs(b)) return DBL_MAX;
    return a + b;
}

double safe_multiply(double a, double b) {
    if (fabs(b) < 1.0) {
        if (fabs(a) > DBL_MAX * fabs(b)) return DBL_MAX;
    } else {
        if (fabs(a) > DBL_MAX / fabs(b)) return DBL_MAX;
    }
    return a * b;
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    double sum = 0.0;
    double product = 1.0;
    double min_val = DBL_MAX;
    double max_val = -DBL_MAX;

    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");

    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        
        if (input[0] == '\0') break;
        
        if (!is_valid_double(input)) {
            printf("Invalid number. Please enter a valid number.\n");
            continue;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\0') {
            printf("Invalid number. Please enter a valid number.\n");
            continue;
        }
        
        if (num != num) {
            printf("Invalid number (NaN). Please enter a valid number.\n");
            continue;
        }
        
        numbers[count] = num;
        sum = safe_add(sum, num);
        product = safe_multiply(product, num);
        
        if (num < min_val) min_val = num;
        if (num > max_val) max_val = num;
        
        count++;
    }

    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }

    double mean = sum / count;
    double variance = 0.0;
    
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance = safe_add(variance, diff * diff);
    }
    variance /= count;
    double stddev = sqrt(variance);

    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);

    return 0;
}