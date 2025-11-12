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
    
    if (n > INT_MAX / 1000) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    uint64_t sum = 0;
    uint64_t product = 1;
    
    for (int i = 1; i <= n; i++) {
        if (i > 0 && product > UINT64_MAX / i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        product *= i;
        sum += i * i;
    }
    
    double pi_approx = 0.0;
    for (int k = 0; k < MAX_ITERATIONS; k++) {
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2.0 * k + 1.0);
        pi_approx += term;
    }
    pi_approx *= 4.0;
    
    double result = (double)sum / (double)product * pi_approx;
    
    printf("Input: %d\n", n);
    printf("Sum of squares: %lu\n", sum);
    printf("Factorial: %lu\n", product);
    printf("Pi approximation: %.10f\n", pi_approx);
    printf("Final result: %.10f\n", result);
    
    return 0;
}