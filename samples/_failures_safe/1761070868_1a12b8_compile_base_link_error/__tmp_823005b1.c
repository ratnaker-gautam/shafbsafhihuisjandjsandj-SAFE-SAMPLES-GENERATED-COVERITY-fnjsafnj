//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    int dot_count = 0;
    int digit_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str) {
        if (*str == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (isdigit(*str)) {
            digit_count++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
}

double safe_power(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    
    double result = pow(base, exponent);
    if (isnan(result) || isinf(result)) return NAN;
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
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (!is_valid_number(input)) {
            printf("Invalid number: %s\n", input);
            continue;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Invalid number format: %s\n", input);
            continue;
        }
        
        if (isnan(num) || isinf(num)) {
            printf("Number out of representable range: %s\n", input);
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("\nProcessing %d numbers...\n", count);
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        double val = numbers[i];
        sum += val;
        product *= val;
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    
    double mean = sum / count;
    
    double sum_squares = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        sum_squares += diff * diff;
    }
    double std_dev = sqrt(sum_squares / count);
    
    printf("Results:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    if (count >= 2) {
        printf("\nComputing powers between consecutive numbers:\n");
        for (int i = 0; i < count - 1; i++) {
            double base = numbers[i];
            double exponent = numbers[i + 1];
            double result = safe_power(base, exponent);
            
            if (isnan(result)) {
                printf("Power(%.6f, %.6f): undefined\n", base, exponent);
            } else {
                printf("Power(%.6f, %.6f) = %.6f\n", base, exponent, result);
            }
        }
    }
    
    return 0;
}