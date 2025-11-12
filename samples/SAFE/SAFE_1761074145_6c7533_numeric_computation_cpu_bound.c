//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        fprintf(stderr, "Error: Number must be between 1 and 1000\n");
        return 1;
    }
    
    if (n > INT_MAX / 100) {
        fprintf(stderr, "Error: Number too large for safe computation\n");
        return 1;
    }
    
    uint64_t sum = 0;
    uint64_t prev_sum = 0;
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        for (int j = 1; j <= n; j++) {
            uint64_t term = (uint64_t)j * j * i;
            if (term > UINT64_MAX - sum) {
                fprintf(stderr, "Error: Overflow detected\n");
                return 1;
            }
            prev_sum = sum;
            sum += term;
        }
    }
    
    printf("Computed sum: %lu\n", (unsigned long)sum);
    
    double average = (double)sum / (double)(MAX_ITERATIONS * n);
    printf("Average term value: %.6f\n", average);
    
    uint64_t verification = 0;
    for (int i = 1; i <= n; i++) {
        uint64_t square = (uint64_t)i * i;
        if (square > UINT64_MAX / MAX_ITERATIONS) {
            fprintf(stderr, "Error: Verification overflow\n");
            return 1;
        }
        uint64_t partial = square * MAX_ITERATIONS;
        if (partial > UINT64_MAX - verification) {
            fprintf(stderr, "Error: Verification overflow\n");
            return 1;
        }
        verification += partial;
    }
    
    uint64_t total_iterations = (uint64_t)MAX_ITERATIONS * n;
    uint64_t expected_sum = ((uint64_t)n * (n + 1) * (2 * n + 1)) / 6;
    if (expected_sum > UINT64_MAX / MAX_ITERATIONS) {
        fprintf(stderr, "Error: Expected sum overflow\n");
        return 1;
    }
    expected_sum *= MAX_ITERATIONS;
    
    printf("Verification sum: %lu\n", (unsigned long)verification);
    printf("Expected sum: %lu\n", (unsigned long)expected_sum);
    
    if (sum == verification && sum == expected_sum) {
        printf("Results verified successfully\n");
    } else {
        printf("Results verification failed\n");
        return 1;
    }
    
    return 0;
}