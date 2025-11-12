//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    if (b == 0.0) return 0.0;
    if (fabs(a) > DBL_MAX / fabs(b)) return DBL_MAX;
    return a * b;
}

double safe_power(double base, double exponent) {
    if (base == 0.0 && exponent < 0) return DBL_MAX;
    if (fabs(base) > 1.0 && exponent > log(DBL_MAX) / log(fabs(base))) return DBL_MAX;
    return pow(base, exponent);
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    double sum = 0.0;
    double product = 1.0;
    double min_val = DBL_MAX;
    double max_val = -DBL_MAX;

    printf("Enter up to 100 numbers (type 'done' to finish):\n");
    
    while (count < 100) {
        printf("Number %d: ", count + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "done") == 0) break;
        
        if (!is_valid_double(input)) {
            printf("Invalid number. Please enter a valid number or 'done'.\n");
            continue;
        }
        
        double num = strtod(input, NULL);
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
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    double mean = sum / count;
    printf("Mean: %.6f\n", mean);
    
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance = safe_add(variance, safe_multiply(diff, diff));
    }
    variance /= count;
    double stddev = sqrt(variance);
    printf("Standard Deviation: %.6f\n", stddev);
    
    double geometric_mean = safe_power(product, 1.0 / count);
    printf("Geometric Mean: %.6f\n", geometric_mean);
    
    double harmonic_sum = 0.0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] == 0.0) {
            harmonic_sum = DBL_MAX;
            break;
        }
        harmonic_sum = safe_add(harmonic_sum, 1.0 / numbers[i]);
    }
    double harmonic_mean = count / harmonic_sum;
    printf("Harmonic Mean: %.6f\n", harmonic_mean);
    
    return 0;
}