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
    if (arr == NULL || size < 1) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *ptr = (i * 3 + 7) % 23;
        ptr++;
    }
}

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size < 1) return;
    
    int* ptr = arr;
    long long sum = 0;
    long long sum_squares = 0;
    
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        sum_squares += (*ptr) * (*ptr);
        ptr++;
    }
    
    results[0] = (double)sum / size;
    results[1] = sqrt(((double)sum_squares / size) - (results[0] * results[0]));
}

void reverse_array(int* arr, int size) {
    if (arr == NULL || size < 1) return;
    
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

void print_array(int* arr, int size) {
    if (arr == NULL || size < 1) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d", *ptr);
        if (i < size - 1) printf(" ");
        ptr++;
    }
    printf("\n");
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (validate_input(input, &size) != 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    double stats[2];
    
    fill_array(arr, size);
    
    printf("Original array: ");
    print_array(arr, size);
    
    compute_statistics(arr, size, stats);
    printf("Mean: %.2f\n", stats[0]);
    printf("Std Dev: %.2f\n", stats[1]);
    
    reverse_array(arr, size);
    printf("Reversed array: ");
    print_array(arr, size);
    
    return 0;
}