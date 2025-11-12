//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    double sum = 0.0;
    double product = 1.0;
    double min_val = 0.0;
    double max_val = 0.0;
    int first = 1;

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
            printf("Invalid number. Please enter a valid number.\n");
            continue;
        }
        
        double num = strtod(input, NULL);
        
        if (num == HUGE_VAL || num == -HUGE_VAL) {
            printf("Number out of range. Please enter a valid number.\n");
            continue;
        }
        
        numbers[count] = num;
        
        if (first) {
            min_val = num;
            max_val = num;
            first = 0;
        } else {
            if (num < min_val) min_val = num;
            if (num > max_val) max_val = num;
        }
        
        sum += num;
        product *= num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nResults:\n");
    printf("Numbers entered: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    double mean = sum / count;
    printf("Mean: %.6f\n", mean);
    
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    double stddev = sqrt(variance);
    printf("Standard deviation: %.6f\n", stddev);
    
    return 0;
}