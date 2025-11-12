//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT];
    double numbers[100];
    int count = 0;
    double sum = 0.0;
    double product = 1.0;
    double min_val, max_val;
    int first_valid = 0;

    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n' || input[0] == '\0') {
            break;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\n' && *endptr != '\0') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (!isfinite(num)) {
            printf("Invalid number. Please enter a finite value.\n");
            continue;
        }
        
        numbers[count] = num;
        
        if (!first_valid) {
            min_val = num;
            max_val = num;
            first_valid = 1;
        } else {
            if (num < min_val) min_val = num;
            if (num > max_val) max_val = num;
        }
        
        sum += num;
        
        if (fabs(num) > 1e-100 && fabs(product) < 1e100) {
            product *= num;
        }
        
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
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
    int i;
    for (i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    double sorted[100];
    memcpy(sorted, numbers, count * sizeof(double));
    
    for (i = 0; i < count - 1; i++) {
        int j;
        for (j = 0; j < count - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                double temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    double median;
    if (count % 2 == 0) {
        median = (sorted[count / 2 - 1] + sorted[count / 2]) / 2.0;
    } else {
        median = sorted[count / 2];
    }
    
    printf("Median: %.6f\n", median);
    
    return 0;
}