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
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing sum of squares for first %d natural numbers...\n", n);
    
    uint64_t sum = 0;
    for (int i = 1; i <= n; i++) {
        if (i > UINT64_MAX / i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        uint64_t square = (uint64_t)i * (uint64_t)i;
        if (sum > UINT64_MAX - square) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum += square;
    }
    
    printf("Sum of squares: %lu\n", sum);
    
    printf("Performing additional CPU-intensive computation...\n");
    double result = 0.0;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double x = (double)iter / 1000.0;
        for (int j = 0; j < 100; j++) {
            result += x * x + 2.0 * x + 1.0;
            result = result / 2.0;
        }
    }
    
    printf("Final computation result: %f\n", result);
    return 0;
}