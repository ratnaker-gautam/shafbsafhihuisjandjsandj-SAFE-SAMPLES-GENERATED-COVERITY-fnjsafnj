//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

uint64_t collatz_sequence_length(uint64_t n) {
    uint64_t length = 1;
    while (n != 1) {
        if (n % 2 == 0) {
            if (n > UINT64_MAX / 2) {
                return 0;
            }
            n = n / 2;
        } else {
            if (n > (UINT64_MAX - 1) / 3) {
                return 0;
            }
            n = 3 * n + 1;
        }
        length++;
        if (length > MAX_ITERATIONS) {
            return 0;
        }
    }
    return length;
}

int main(void) {
    uint64_t start, end;
    char input_buffer[256];
    
    printf("Enter start number (1-100000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%lu", &start) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Enter end number (1-100000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input_buffer, "%lu", &end) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (start < 1 || end < 1 || start > 100000 || end > 100000) {
        fprintf(stderr, "Numbers must be between 1 and 100000\n");
        return 1;
    }
    
    if (start > end) {
        fprintf(stderr, "Start number must be less than or equal to end number\n");
        return 1;
    }
    
    uint64_t max_length = 0;
    uint64_t max_number = 0;
    uint64_t valid_count = 0;
    
    for (uint64_t i = start; i <= end; i++) {
        uint64_t length = collatz_sequence_length(i);
        if (length == 0) {
            printf("Sequence for %lu exceeded maximum iterations or overflowed\n", i);
            continue;
        }
        valid_count++;
        if (length > max_length) {
            max_length = length;
            max_number = i;
        }
    }
    
    if (valid_count > 0) {
        printf("Analyzed %lu numbers from %lu to %lu\n", valid_count, start, end);
        printf("Longest Collatz sequence: %lu steps for number %lu\n", max_length, max_number);
    } else {
        printf("No valid sequences found in range %lu to %lu\n", start, end);
    }
    
    return 0;
}