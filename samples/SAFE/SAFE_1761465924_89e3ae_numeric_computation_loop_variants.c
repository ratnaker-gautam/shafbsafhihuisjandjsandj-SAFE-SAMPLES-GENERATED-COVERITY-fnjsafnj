//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
        
        if (num == HUGE_VAL || num == -HUGE_VAL || num != num) {
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
    
    printf("\nProcessing %d numbers...\n", count);
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    
    int j = 0;
    while (j < count) {
        product *= numbers[j];
        j++;
    }
    
    int k = 0;
    do {
        if (numbers[k] < min_val) {
            min_val = numbers[k];
        }
        if (numbers[k] > max_val) {
            max_val = numbers[k];
        }
        k++;
    } while (k < count);
    
    double mean = sum / count;
    double variance = 0.0;
    
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    printf("Results:\n");
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    
    return 0;
}