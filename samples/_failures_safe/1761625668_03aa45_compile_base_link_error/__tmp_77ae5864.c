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
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (!is_valid_double(input)) {
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }
        
        char* endptr;
        double num = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }
        
        if (isnan(num) || isinf(num)) {
            printf("Number out of representable range.\n");
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
        } else {
            double old_product = product;
            product *= val;
            if (isinf(product) || isnan(product) || 
                (fabs(old_product) > 1.0 && fabs(product) < 1e-300)) {
                product = NAN;
                break;
            }
        }
        
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    
    double mean = sum / count;
    
    double sum_sq_diff = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        sum_sq_diff += diff * diff;
    }
    double std_dev = sqrt(sum_sq_diff / count);
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    
    if (isnan(product)) {
        printf("Product: undefined (overflow/underflow)\n");
    } else {
        printf("Product: %.6f\n", product);
    }
    
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    if (count >= 2) {
        printf("\nPairwise computations:\n");
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                double a = numbers[i];
                double b = numbers[j];
                
                if (b != 0.0) {
                    printf("(%d,%d): a/b=%.3f", i, j, a/b);
                } else {
                    printf("(%d,%d): a/b=undefined", i, j);
                }
                
                double power = safe_power(a, b);
                if (isnan(power)) {
                    printf(", a^b=undefined\n");
                } else {
                    printf(", a^b=%.3f\n", power);
                }
            }
        }
    }
    
    return 0;
}