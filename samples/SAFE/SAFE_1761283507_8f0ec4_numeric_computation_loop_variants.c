//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    printf("Computing sum of first %d triangular numbers...\n", n);
    
    uint64_t sum = 0;
    int i = 1;
    
    while (i <= n) {
        if (i > UINT64_MAX / (i + 1) / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        uint64_t triangular = (uint64_t)i * (i + 1) / 2;
        
        if (sum > UINT64_MAX - triangular) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum += triangular;
        i++;
    }
    
    printf("Result using while loop: %lu\n", sum);
    
    sum = 0;
    for (int j = 1; j <= n; j++) {
        if (j > UINT64_MAX / (j + 1) / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        uint64_t triangular = (uint64_t)j * (j + 1) / 2;
        
        if (sum > UINT64_MAX - triangular) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum += triangular;
    }
    
    printf("Result using for loop: %lu\n", sum);
    
    sum = 0;
    int k = 1;
    do {
        if (k > UINT64_MAX / (k + 1) / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        uint64_t triangular = (uint64_t)k * (k + 1) / 2;
        
        if (sum > UINT64_MAX - triangular) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum += triangular;
        k++;
    } while (k <= n);
    
    printf("Result using do-while loop: %lu\n", sum);
    
    return 0;
}