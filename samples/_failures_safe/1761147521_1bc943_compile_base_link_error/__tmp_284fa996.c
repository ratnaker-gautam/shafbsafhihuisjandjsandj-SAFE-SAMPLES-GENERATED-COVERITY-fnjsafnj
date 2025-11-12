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
        
        char *endptr;
        double num = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (num == HUGE_VAL || num == -HUGE_VAL) {
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
    double variance = 0.0;
    
    int j = 0;
    while (j < count) {
        double diff = numbers[j] - mean;
        variance += diff * diff;
        j++;
    }
    variance /= count;
    
    double std_dev = sqrt(variance);
    
    printf("Statistics:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("Numbers in reverse order:\n");
    int k = count - 1;
    do {
        printf("%.6f\n", numbers[k]);
        k--;
    } while (k >= 0);
    
    return 0;
}