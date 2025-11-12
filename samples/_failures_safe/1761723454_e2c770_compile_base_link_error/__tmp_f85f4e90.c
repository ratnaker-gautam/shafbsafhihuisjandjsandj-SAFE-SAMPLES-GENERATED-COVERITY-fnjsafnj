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
    
    int dot_count = 0;
    int digit_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            digit_count++;
        } else if (*str == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
}

double safe_pow(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    
    double result = pow(base, exponent);
    if (!isfinite(result)) return NAN;
    return result;
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[10];
    int count = 0;
    
    printf("Enter up to 10 numbers (or 'done' to finish):\n");
    
    while (count < 10) {
        printf("Number %d: ", count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (!is_valid_double(input)) {
            printf("Invalid number format\n");
            continue;
        }
        
        char* endptr;
        double num = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Invalid number format\n");
            continue;
        }
        
        if (!isfinite(num)) {
            printf("Number out of representable range\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered\n");
        return 0;
    }
    
    printf("\nComputing statistics for %d numbers:\n", count);
    
    double sum = 0.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
    }
    
    double mean = sum / count;
    
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    double stddev = sqrt(variance);
    
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", stddev);
    
    printf("\nComputing powers:\n");
    for (int i = 0; i < count; i++) {
        double square = safe_pow(numbers[i], 2.0);
        double cube = safe_pow(numbers[i], 3.0);
        double sqrt_val = (numbers[i] >= 0.0) ? sqrt(numbers[i]) : NAN;
        
        printf("Number %.6f:\n", numbers[i]);
        printf("  Square: ");
        if (isfinite(square)) printf("%.6f", square);
        else printf("undefined");
        printf("\n");
        
        printf("  Cube: ");
        if (isfinite(cube)) printf("%.6f", cube);
        else printf("undefined");
        printf("\n");
        
        printf("  Square root: ");
        if (isfinite(sqrt_val)) printf("%.6f", sqrt_val);
        else printf("undefined");
        printf("\n");
    }
    
    return 0;
}