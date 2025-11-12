//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT_LEN 100

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter numbers (one per line, 'done' to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        char *endptr;
        double num = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input. Enter a number or 'done': ");
            continue;
        }
        
        if (num != num) {
            printf("Invalid number. Try again: ");
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
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
        
        if (numbers[i] != 0.0) {
            product *= numbers[i];
        }
        
        if (numbers[i] < min_val) {
            min_val = numbers[i];
        }
        
        if (numbers[i] > max_val) {
            max_val = numbers[i];
        }
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    int j = 0;
    while (j < count) {
        double diff = numbers[j] - mean;
        variance_sum += diff * diff;
        j++;
    }
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    double geometric_mean;
    if (product >= 0.0) {
        geometric_mean = pow(product, 1.0 / count);
    } else {
        geometric_mean = -pow(-product, 1.0 / count);
    }
    
    printf("\nResults for %d numbers:\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nNumbers in reverse order:\n");
    int k = count - 1;
    do {
        printf("%.6f\n", numbers[k]);
        k--;
    } while (k >= 0);
    
    return 0;
}