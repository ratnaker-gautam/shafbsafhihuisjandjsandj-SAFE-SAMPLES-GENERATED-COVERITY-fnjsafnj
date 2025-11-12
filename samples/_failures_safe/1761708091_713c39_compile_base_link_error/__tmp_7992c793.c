//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    
    int decimal_points = 0;
    int digits = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str != '\0') {
        if (*str == '.') {
            decimal_points++;
            if (decimal_points > 1) return 0;
        } else if (*str >= '0' && *str <= '9') {
            digits++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digits > 0;
}

double safe_power(double base, double exponent) {
    if (base == 0.0 && exponent <= 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    
    double result = pow(base, exponent);
    if (!isfinite(result)) return NAN;
    return result;
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
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }
        
        char* endptr;
        double num = strtod(input, &endptr);
        if (*endptr != '\0' || !isfinite(num)) {
            printf("Invalid number value. Please enter a finite number.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nProcessing %d numbers...\n", count);
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        double val = numbers[i];
        sum += val;
        
        if (fabs(val) < 1e-300) {
            product = 0.0;
        } else if (fabs(product) > 1e300 / fabs(val)) {
            product = (product > 0) ? INFINITY : -INFINITY;
        } else {
            product *= val;
        }
        
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nComputing powers and roots...\n");
    for (int i = 0; i < count && i < 5; i++) {
        double base = numbers[i];
        double square = safe_power(base, 2.0);
        double cube = safe_power(base, 3.0);
        double sqrt_val = safe_power(base, 0.5);
        
        printf("Number %.6f: ", base);
        printf("Square=%.6f, ", square);
        printf("Cube=%.6f, ", cube);
        printf("Sqrt=%.6f\n", sqrt_val);
    }
    
    return 0;
}