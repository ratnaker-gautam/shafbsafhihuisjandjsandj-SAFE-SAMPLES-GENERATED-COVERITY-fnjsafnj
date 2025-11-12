//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int validate_input(const char* input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    
    int has_digit = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (i == 0 && input[i] == '-') continue;
        if (input[i] < '0' || input[i] > '9') return 0;
        has_digit = 1;
    }
    return has_digit;
}

long safe_strtol(const char* str, int* valid) {
    char* endptr;
    long result = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        *valid = 0;
        return 0;
    }
    
    if (result == LONG_MAX || result == LONG_MIN) {
        *valid = 0;
        return 0;
    }
    
    *valid = 1;
    return result;
}

int main() {
    char input[MAX_INPUT + 1];
    long numbers[10];
    int count = 0;
    
    printf("Enter up to 10 integers (empty line to stop):\n");
    
    while (count < 10) {
        printf("Number %d: ", count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            break;
        }
        
        if (!validate_input(input)) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        int valid;
        long num = safe_strtol(input, &valid);
        if (!valid) {
            printf("Number out of range or invalid.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nComputing statistics...\n");
    
    long sum = 0;
    long min = numbers[0];
    long max = numbers[0];
    double sum_squares = 0.0;
    
    int i = 0;
    while (i < count) {
        long num = numbers[i];
        sum += num;
        if (num < min) min = num;
        if (num > max) max = num;
        sum_squares += (double)num * (double)num;
        i++;
    }
    
    double mean = (double)sum / count;
    
    double variance = 0.0;
    for (int j = 0; j < count; j++) {
        double diff = (double)numbers[j] - mean;
        variance += diff * diff;
    }
    variance /= count;
    double stddev = sqrt(variance);
    
    printf("Count: %d\n", count);
    printf("Sum: %ld\n", sum);
    printf("Mean: %.2f\n", mean);
    printf("Min: %ld\n", min);
    printf("Max: %ld\n", max);
    printf("Standard deviation: %.2f\n", stddev);
    
    printf("\nNumber analysis:\n");
    int k = 0;
    do {
        long num = numbers[k];
        printf("Number %d: %ld", k + 1, num);
        
        if (num % 2 == 0) {
            printf(" (even)");
        } else {
            printf(" (odd)");
        }
        
        if (num > 0) {
            printf(" positive");
        } else if (num < 0) {
            printf(" negative");
        } else {
            printf(" zero");
        }
        
        printf("\n");
        k++;
    } while (k < count);
    
    return 0;
}