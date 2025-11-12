//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    long long sum = 0;
    long long square_sum = 0;
    double harmonic_sum = 0.0;
    
    int i = 1;
    while (i <= n) {
        sum += i;
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        square_sum += (long long)j * j;
    }
    
    int k = 1;
    do {
        harmonic_sum += 1.0 / k;
        k++;
    } while (k <= n);
    
    double geometric_mean = exp(log((double)n) / n);
    
    printf("Sum of first %d natural numbers: %lld\n", n, sum);
    printf("Sum of squares: %lld\n", square_sum);
    printf("Harmonic sum: %.6f\n", harmonic_sum);
    printf("Geometric mean of 1 to %d: %.6f\n", n, geometric_mean);
    
    int iterations = 0;
    double x = 1.0;
    while (iterations < MAX_ITERATIONS) {
        double new_x = 0.5 * (x + n / x);
        if (fabs(new_x - x) < 1e-10) {
            break;
        }
        x = new_x;
        iterations++;
    }
    
    printf("Approximate square root of %d: %.6f\n", n, x);
    printf("Iterations needed: %d\n", iterations);
    
    return 0;
}