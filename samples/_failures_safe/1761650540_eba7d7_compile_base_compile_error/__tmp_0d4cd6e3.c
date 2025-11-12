//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT];
    double numbers[10];
    int count = 0;
    
    printf("Enter up to 10 numbers (one per line, 'done' to finish):\n");
    
    while (count < 10) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input. Enter a number or 'done': ");
            continue;
        }
        
        if (!isfinite(num)) {
            printf("Number out of range. Try again: ");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nProcessing %d numbers:\n", count);
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
        
        if (fabs(product) > 1e-300 && fabs(numbers[i]) > 1e-300) {
            product *= numbers[i];
        }
        
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
    double std_dev = sqrt(variance);
    
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    
    printf("\nSquared values:\n");
    int j = 0;
    do {
        if (j < count) {
            double squared = numbers[j] * numbers[j];
            printf("  %.6f^2 = %.6f\n", numbers[j], squared);
            j++;
        }
    } while (j < count);
    
    printf("\nCubed values:\n");
    int k = 0;
    while (k < count) {
        double cubed = numbers[k] * numbers[k] * numbers[k];
        printf("  %.6f^3 = %.6f\n", numbers[k], cubed);
        k++;
    }
    
    printf("\nFactorial approximations (for positive integers <= 20):\n");
    for (int m = 0; m < count; m++) {
        if (numbers[m] >= 0 && numbers[m] <= 20 && fabs(numbers[m] - round(numbers[m])) < 1e-10) {
            int n = (int)round(numbers[m]);
            long long factorial = 1;
            
            for (int p = 1; p <= n; p++) {
                if (factorial > LLONG_MAX / p) {
                    factorial = -1;
                    break;
                }
                factorial *= p;
            }
            
            if (factorial == -1) {
                printf("  %d! = overflow\n", n);
            } else {
                printf("  %d! = %lld\n", n, factorial);
            }
        }
    }
    
    return 0;
}