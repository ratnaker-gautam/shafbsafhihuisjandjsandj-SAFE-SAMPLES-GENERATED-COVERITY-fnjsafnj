//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int validate_input(const char *input) {
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

long safe_strtol(const char *str, int *valid) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        *valid = 0;
        return 0;
    }
    
    if (val == LONG_MAX || val == LONG_MIN) {
        *valid = 0;
        return 0;
    }
    
    *valid = 1;
    return val;
}

int main() {
    char input[MAX_INPUT + 1];
    long numbers[10];
    int count = 0;
    
    printf("Enter up to 10 integers (empty line to stop):\n");
    
    while (count < 10) {
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
    
    printf("\nProcessing numbers:\n");
    
    long sum = 0;
    long product = 1;
    int has_overflow = 0;
    
    int i = 0;
    while (i < count) {
        sum += numbers[i];
        if ((numbers[i] > 0 && product > LONG_MAX / numbers[i]) ||
            (numbers[i] < 0 && product < LONG_MIN / numbers[i])) {
            has_overflow = 1;
        } else {
            product *= numbers[i];
        }
        i++;
    }
    
    double geometric_mean;
    if (count > 0) {
        double log_sum = 0.0;
        int j = 0;
        do {
            if (numbers[j] <= 0) {
                geometric_mean = 0.0;
                break;
            }
            log_sum += log((double)numbers[j]);
            j++;
        } while (j < count);
        
        if (j == count) {
            geometric_mean = exp(log_sum / count);
        }
    } else {
        geometric_mean = 0.0;
    }
    
    long min_val = numbers[0];
    long max_val = numbers[0];
    for (int k = 1; k < count; k++) {
        if (numbers[k] < min_val) min_val = numbers[k];
        if (numbers[k] > max_val) max_val = numbers[k];
    }
    
    printf("Sum: %ld\n", sum);
    if (has_overflow) {
        printf("Product: overflow occurred\n");
    } else {
        printf("Product: %ld\n", product);
    }
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Minimum: %ld\n", min_val);
    printf("Maximum: %ld\n", max_val);
    
    printf("\nNumber analysis:\n");
    int m = 0;
    while (m < count) {
        printf("Number %d: %ld - ", m + 1, numbers[m]);
        
        if (numbers[m] == 0) {
            printf("zero");
        } else if (numbers[m] > 0) {
            printf("positive");
        } else {
            printf("negative");
        }
        
        if (numbers[m] != 0) {
            if (numbers[m] % 2 == 0) {
                printf(" even");
            } else {
                printf(" odd");
            }
        }
        
        printf("\n");
        m++;
    }
    
    return 0;
}