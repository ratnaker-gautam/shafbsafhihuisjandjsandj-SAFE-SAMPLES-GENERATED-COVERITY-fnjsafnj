//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char *input, int *valid) {
    if (input == NULL || valid == NULL) return -1;
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') return -1;
    if (val < 1 || val > MAX_SIZE) return -1;
    *valid = (int)val;
    return 0;
}

void compute_squares(int *arr, size_t n) {
    if (arr == NULL || n == 0) return;
    int *ptr = arr;
    for (size_t i = 0; i < n; i++) {
        *ptr = (*ptr) * (*ptr);
        if (*ptr < 0) *ptr = 0;
        ptr++;
    }
}

double compute_mean(const int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0.0;
    long long sum = 0;
    const int *ptr = arr;
    for (size_t i = 0; i < n; i++) {
        sum += *ptr;
        ptr++;
    }
    return (double)sum / (double)n;
}

int main() {
    char buffer[256];
    int n;
    
    printf("Enter number of elements (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (validate_input(buffer, &n) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        int value;
        if (validate_input(buffer, &value) != 0) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        
        *(arr + i) = value;
    }
    
    compute_squares(arr, n);
    
    printf("Squared values: ");
    int *ptr = arr;
    for (int i = 0; i < n; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double mean = compute_mean(arr, n);
    printf("Mean of squared values: %.2f\n", mean);
    
    return 0;
}