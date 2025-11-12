//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    
    *valid = (int)value;
    return 0;
}

void fill_array(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *ptr = (i * 2) + 1;
        ptr++;
    }
}

double compute_stats(int* arr, int size, int* min_val, int* max_val) {
    if (arr == NULL || size < 1 || size > MAX_SIZE || min_val == NULL || max_val == NULL) {
        return 0.0;
    }
    
    int* ptr = arr;
    int sum = 0;
    *min_val = *ptr;
    *max_val = *ptr;
    
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        if (*ptr < *min_val) *min_val = *ptr;
        if (*ptr > *max_val) *max_val = *ptr;
        ptr++;
    }
    
    return (double)sum / size;
}

void reverse_array(int* arr, int size) {
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* start = arr;
    int* end = arr + size - 1;
    
    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (validate_input(input, &size) != 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    printf("Original array: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    int min_val, max_val;
    double average = compute_stats(arr, size, &min_val, &max_val);
    
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Average: %.2f\n", average);
    
    reverse_array(arr, size);
    
    printf("Reversed array: ");
    ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}