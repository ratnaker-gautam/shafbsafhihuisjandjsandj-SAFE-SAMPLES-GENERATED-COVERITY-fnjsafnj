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

void compute_statistics(int* data, size_t size, double* results) {
    if (data == NULL || results == NULL || size == 0) return;
    
    int* ptr = data;
    double sum = 0.0;
    double sum_squares = 0.0;
    int min = *ptr;
    int max = *ptr;
    
    for (size_t i = 0; i < size; i++) {
        int value = *(ptr + i);
        sum += value;
        sum_squares += value * value;
        if (value < min) min = value;
        if (value > max) max = value;
    }
    
    results[0] = sum / size;
    results[1] = sqrt((sum_squares / size) - ((sum / size) * (sum / size)));
    results[2] = (double)min;
    results[3] = (double)max;
}

void fill_with_primes(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* current = arr;
    int count = 0;
    int num = 2;
    
    while (count < size) {
        int is_prime = 1;
        for (int i = 2; i <= (int)sqrt(num); i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime) {
            *(current + count) = num;
            count++;
        }
        num++;
    }
}

int main() {
    char input_buffer[32];
    int size;
    
    printf("Enter number of elements (1-%d): ", MAX_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input_buffer, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int data[MAX_SIZE];
    double results[4];
    
    fill_with_primes(data, size);
    compute_statistics(data, size, results);
    
    printf("Generated prime numbers:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", *(data + i));
    }
    printf("\n\n");
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Standard Deviation: %.2f\n", results[1]);
    printf("Minimum: %.0f\n", results[2]);
    printf("Maximum: %.0f\n", results[3]);
    
    return 0;
}