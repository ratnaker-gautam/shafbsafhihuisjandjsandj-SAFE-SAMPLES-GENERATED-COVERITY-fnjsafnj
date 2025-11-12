//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 10

int main() {
    int numbers[ARRAY_SIZE];
    int sum = 0;
    long long product = 1;
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    
    printf("Enter %d integers:\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        char buffer[32];
        char *endptr;
        long temp;
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            fprintf(stderr, "Invalid input: must be an integer\n");
            return 1;
        }
        
        if (temp < INT_MIN || temp > INT_MAX) {
            fprintf(stderr, "Input out of integer range\n");
            return 1;
        }
        
        numbers[i] = (int)temp;
    }
    
    int i = 0;
    while (i < ARRAY_SIZE) {
        sum += numbers[i];
        i++;
    }
    
    int j = 0;
    do {
        if (numbers[j] != 0) {
            if (product > LLONG_MAX / abs(numbers[j])) {
                fprintf(stderr, "Product would overflow\n");
                return 1;
            }
            product *= numbers[j];
        }
        j++;
    } while (j < ARRAY_SIZE);
    
    for (int k = 0; k < ARRAY_SIZE; k++) {
        if (numbers[k] < min_val) {
            min_val = numbers[k];
        }
        if (numbers[k] > max_val) {
            max_val = numbers[k];
        }
    }
    
    double average = (double)sum / ARRAY_SIZE;
    
    printf("Sum: %d\n", sum);
    printf("Product: %lld\n", product);
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Average: %.2f\n", average);
    
    printf("Squared values: ");
    for (int m = 0; m < ARRAY_SIZE; m++) {
        long long square = (long long)numbers[m] * numbers[m];
        if (square > INT_MAX) {
            printf("[%lld] ", square);
        } else {
            printf("%lld ", square);
        }
    }
    printf("\n");
    
    return 0;
}