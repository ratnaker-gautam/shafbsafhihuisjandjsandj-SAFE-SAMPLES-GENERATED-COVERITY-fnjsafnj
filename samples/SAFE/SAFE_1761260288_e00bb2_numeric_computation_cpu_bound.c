//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_valid_input(long long n) {
    return (n > 0 && n <= 1000000);
}

long long compute_collatz_length(long long n) {
    long long length = 1;
    while (n != 1) {
        if (n % 2 == 0) {
            if (n > LLONG_MAX / 2) {
                return -1;
            }
            n = n / 2;
        } else {
            if (n > (LLONG_MAX - 1) / 3) {
                return -1;
            }
            n = 3 * n + 1;
        }
        length++;
        if (length > MAX_ITERATIONS) {
            return -1;
        }
    }
    return length;
}

int main(void) {
    long long start_num;
    char input_buffer[64];
    
    printf("Enter starting number (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    start_num = strtoll(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!is_valid_input(start_num)) {
        fprintf(stderr, "Number out of valid range\n");
        return 1;
    }
    
    long long max_length = 0;
    long long max_number = 0;
    
    for (long long i = 1; i <= start_num; i++) {
        long long length = compute_collatz_length(i);
        if (length == -1) {
            fprintf(stderr, "Computation overflow for number %lld\n", i);
            return 1;
        }
        
        if (length > max_length) {
            max_length = length;
            max_number = i;
        }
    }
    
    printf("Number with longest Collatz sequence up to %lld: %lld\n", start_num, max_number);
    printf("Sequence length: %lld\n", max_length);
    
    return 0;
}