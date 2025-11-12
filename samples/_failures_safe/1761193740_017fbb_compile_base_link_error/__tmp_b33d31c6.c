//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT_LEN 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    double numbers[100];
    int count = 0;
    
    printf("Enter numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n') {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        int valid = 1;
        int decimal_points = 0;
        int has_digit = 0;
        
        for (size_t i = 0; i < len; i++) {
            if (input[i] == '-' && i == 0) {
                continue;
            }
            if (input[i] == '.') {
                decimal_points++;
                if (decimal_points > 1) {
                    valid = 0;
                    break;
                }
                continue;
            }
            if (input[i] >= '0' && input[i] <= '9') {
                has_digit = 1;
                continue;
            }
            valid = 0;
            break;
        }
        
        if (!valid || !has_digit) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        if (endptr != input + len) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    int i = 0;
    while (i < count) {
        sum += numbers[i];
        product *= numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
        i++;
    }
    
    double mean = sum / count;
    
    double variance = 0.0;
    for (int j = 0; j < count; j++) {
        double diff = numbers[j] - mean;
        variance += diff * diff;
    }
    variance /= count;
    double std_dev = sqrt(variance);
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nNumbers squared:\n");
    int k = 0;
    do {
        if (k >= count) break;
        printf("%.6f ", numbers[k] * numbers[k]);
        k++;
    } while (k < count);
    printf("\n");
    
    return 0;
}