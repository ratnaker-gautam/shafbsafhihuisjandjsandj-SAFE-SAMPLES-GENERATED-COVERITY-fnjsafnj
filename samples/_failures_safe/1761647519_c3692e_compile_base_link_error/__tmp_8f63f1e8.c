//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

int main() {
    char input[MAX_SIZE];
    int *numbers = NULL;
    int count = 0;
    
    printf("Enter number of elements (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    count = atoi(input);
    if (count <= 0 || count > MAX_SIZE) {
        printf("Invalid count\n");
        return 1;
    }
    
    numbers = malloc(count * sizeof(int));
    if (numbers == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d numbers:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Number %d: ", i + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            free(numbers);
            return 1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (!validate_input(input)) {
            printf("Invalid number\n");
            free(numbers);
            return 1;
        }
        
        *(numbers + i) = atoi(input);
    }
    
    int *start = numbers;
    int *end = numbers + count - 1;
    int sum = 0;
    double mean = 0.0;
    double variance = 0.0;
    
    int *ptr = start;
    while (ptr <= end) {
        sum += *ptr;
        ptr++;
    }
    
    mean = (double)sum / count;
    
    ptr = start;
    while (ptr <= end) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= count;
    
    double std_dev = sqrt(variance);
    
    printf("\nResults:\n");
    printf("Sum: %d\n", sum);
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", std_dev);
    
    free(numbers);
    return 0;
}