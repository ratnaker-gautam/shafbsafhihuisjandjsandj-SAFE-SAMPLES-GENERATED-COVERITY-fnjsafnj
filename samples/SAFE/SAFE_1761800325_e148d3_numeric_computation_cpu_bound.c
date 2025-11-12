//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
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
    
    long double pi_approx = 0.0L;
    int iterations = n * 1000;
    
    if (iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
    }
    
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double area_sum = 0.0L;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long double x = (long double)i / n;
            long double y = (long double)j / n;
            long double dist_sq = x * x + y * y;
            if (dist_sq <= 1.0L) {
                area_sum += 1.0L;
            }
        }
    }
    
    long double circle_area = area_sum / (n * n);
    long double computed_pi = circle_area * 4.0L;
    
    printf("Pi approximation (Leibniz): %.15Lf\n", pi_approx);
    printf("Pi approximation (Monte Carlo): %.15Lf\n", computed_pi);
    printf("Difference: %.15Lf\n", pi_approx - computed_pi);
    
    long double final_result = (pi_approx + computed_pi) / 2.0L;
    printf("Final combined estimate: %.15Lf\n", final_result);
    
    return 0;
}