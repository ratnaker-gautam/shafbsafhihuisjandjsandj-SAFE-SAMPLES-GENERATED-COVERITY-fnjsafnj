//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* str, int* valid) {
    if (str == NULL) return 0;
    
    int len = 0;
    while (str[len] != '\0') {
        if (str[len] < '0' || str[len] > '9') {
            *valid = 0;
            return 0;
        }
        len++;
        if (len >= MAX_SIZE) {
            *valid = 0;
            return 0;
        }
    }
    
    *valid = 1;
    return len;
}

long safe_strtol(const char* str, int* success) {
    if (str == NULL) {
        *success = 0;
        return 0;
    }
    
    long result = 0;
    const char* ptr = str;
    
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            *success = 0;
            return 0;
        }
        
        if (result > (LONG_MAX - (*ptr - '0')) / 10) {
            *success = 0;
            return 0;
        }
        
        result = result * 10 + (*ptr - '0');
        ptr++;
    }
    
    *success = 1;
    return result;
}

void compute_statistics(long* numbers, int count, long* sum, long* min, long* max) {
    if (numbers == NULL || count <= 0 || sum == NULL || min == NULL || max == NULL) {
        return;
    }
    
    *sum = 0;
    *min = numbers[0];
    *max = numbers[0];
    
    long* ptr = numbers;
    for (int i = 0; i < count; i++) {
        *sum += *ptr;
        if (*ptr < *min) *min = *ptr;
        if (*ptr > *max) *max = *ptr;
        ptr++;
    }
}

int main() {
    char buffer[MAX_SIZE + 1];
    long numbers[MAX_SIZE];
    int count = 0;
    
    printf("Enter up to %d positive integers (empty line to stop):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        int valid_input = 0;
        int input_len = validate_input(buffer, &valid_input);
        
        if (!valid_input || input_len == 0) {
            printf("Invalid input. Please enter only digits.\n");
            continue;
        }
        
        int success = 0;
        long value = safe_strtol(buffer, &success);
        
        if (!success || value < 0) {
            printf("Invalid number or overflow detected.\n");
            continue;
        }
        
        numbers[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    long sum, min, max;
    compute_statistics(numbers, count, &sum, &min, &max);
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %ld\n", sum);
    printf("Minimum: %ld\n", min);
    printf("Maximum: %ld\n", max);
    
    if (count > 0) {
        double average = (double)sum / count;
        printf("Average: %.2f\n", average);
    }
    
    return 0;
}