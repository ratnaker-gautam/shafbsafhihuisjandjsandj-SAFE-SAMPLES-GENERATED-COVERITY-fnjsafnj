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
        
        if (!first_valid) {
            min_val = num;
            max_val = num;
            first_valid = 1;
        } else {
            if (num < min_val) min_val = num;
            if (num > max_val) max_val = num;
        }
        
        numbers[count] = num;
        sum += num;
        product *= num;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
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
    int i;
    for (i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    double geometric_mean = pow(product, 1.0 / count);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double harmonic_sum = 0.0;
    for (i = 0; i < count; i++) {
        if (numbers[i] != 0.0) {
            harmonic_sum += 1.0 / numbers[i];
        }
    }
    
    if (harmonic_sum != 0.0) {
        double harmonic_mean = count / harmonic_sum;
        printf("Harmonic mean: %.6f\n", harmonic_mean);
    } else {
        printf("Harmonic mean: undefined (division by zero)\n");
    }
    
    return 0;
}