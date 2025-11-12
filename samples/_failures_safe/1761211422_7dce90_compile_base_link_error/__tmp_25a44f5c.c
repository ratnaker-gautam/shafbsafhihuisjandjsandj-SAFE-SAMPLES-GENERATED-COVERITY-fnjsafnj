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

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
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
            break;
        }
        
        if (!is_valid_double(input)) {
            printf("Invalid number. Please enter a valid number.\n");
            continue;
        }
        
        double num = strtod(input, NULL);
        if (num == HUGE_VAL || num == -HUGE_VAL) {
            printf("Number out of range. Please enter a finite number.\n");
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
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
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
    double stddev = sqrt(variance);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Standard Deviation: %.6f\n", stddev);
    
    printf("\nEnter a number to find its position in the sorted list: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    if (!is_valid_double(input)) {
        printf("Invalid number entered.\n");
        return 0;
    }
    
    double search_num = strtod(input, NULL);
    if (search_num == HUGE_VAL || search_num == -HUGE_VAL) {
        printf("Number out of range.\n");
        return 0;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                double temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
    
    int position = -1;
    for (int i = 0; i < count; i++) {
        if (numbers[i] == search_num) {
            position = i + 1;
            break;
        }
    }
    
    if (position != -1) {
        printf("Number %.6f found at position %d in sorted list.\n", search_num, position);
    } else {
        printf("Number %.6f not found in the list.\n", search_num);
    }
    
    return 0;
}