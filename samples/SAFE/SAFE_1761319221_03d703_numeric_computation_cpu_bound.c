//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_valid_input(long long n) {
    return n > 0 && n <= 1000000;
}

long long collatz_length(long long n) {
    long long count = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            if (n > LLONG_MAX / 2) return -1;
            n = n / 2;
        } else {
            if (n > (LLONG_MAX - 1) / 3) return -1;
            n = 3 * n + 1;
        }
        count++;
        if (count > MAX_ITERATIONS) return -1;
    }
    return count;
}

int main(void) {
    long long start, end;
    char buffer[256];
    
    printf("Enter start number (1-1000000): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(buffer, "%lld", &start) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Enter end number (1-1000000): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(buffer, "%lld", &end) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!is_valid_input(start) || !is_valid_input(end)) {
        fprintf(stderr, "Numbers must be between 1 and 1000000\n");
        return 1;
    }
    
    if (start > end) {
        fprintf(stderr, "Start number must be <= end number\n");
        return 1;
    }
    
    printf("Collatz sequence lengths from %lld to %lld:\n", start, end);
    
    for (long long i = start; i <= end; i++) {
        long long length = collatz_length(i);
        if (length == -1) {
            printf("%lld: sequence too long or overflow\n", i);
        } else {
            printf("%lld: %lld steps\n", i, length);
        }
    }
    
    return 0;
}