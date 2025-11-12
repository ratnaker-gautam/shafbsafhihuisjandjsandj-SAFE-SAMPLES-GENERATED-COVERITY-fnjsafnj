//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    for (; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (isdigit(str[i])) {
            digit_count++;
        } else {
            return 0;
        }
    }
    return digit_count > 0;
}

double safe_power(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    return pow(base, exponent);
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter numbers (up to 100, 'done' to finish):\n");
    
    while (count < 100) {
        printf("Number %d: ", count + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        if (strcmp(input, "done") == 0) break;
        
        if (!is_valid_number(input)) {
            printf("Invalid number. Please enter a valid number.\n");
            continue;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Conversion error.\n");
            continue;
        }
        
        if (num == HUGE_VAL || num == -HUGE_VAL || num != num) {
            printf("Number out of range.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nComputing statistics for %d numbers:\n", count);
    
    double sum = 0.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        double val = numbers[i];
        sum += val;
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
    
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nComputing powers:\n");
    printf("Enter exponent: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_number(input)) {
        printf("Invalid exponent.\n");
        return 1;
    }
    
    char *endptr;
    double exponent = strtod(input, &endptr);
    if (*endptr != '\0' || exponent == HUGE_VAL || exponent == -HUGE_VAL || exponent != exponent) {
        printf("Invalid exponent.\n");
        return 1;
    }
    
    printf("\nNumber^%.6f:\n", exponent);
    for (int i = 0; i < count; i++) {
        double result = safe_power(numbers[i], exponent);
        if (result != result) {
            printf("%.6f^%.6f = undefined\n", numbers[i], exponent);
        } else {
            printf("%.6f^%.6f = %.6f\n", numbers[i], exponent, result);
        }
    }
    
    return 0;
}