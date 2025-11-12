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
    
    printf("Enter numbers (one per line, 'done' to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
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
    
    int i = 0;
    do {
        sum += numbers[i];
        product *= numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
        i++;
    } while (i < count);
    
    double mean = sum / count;
    
    double variance = 0.0;
    for (int j = 0; j < count; j++) {
        double diff = numbers[j] - mean;
        variance += diff * diff;
    }
    variance /= count;
    double stddev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", stddev);
    
    printf("Numbers in reverse order:\n");
    for (int k = count - 1; k >= 0; k--) {
        printf("%.6f\n", numbers[k]);
    }
    
    return 0;
}