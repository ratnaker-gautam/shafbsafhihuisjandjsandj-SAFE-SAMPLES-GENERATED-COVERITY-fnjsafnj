//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int sign = 1;
    
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            term = 1.0 / (2.0 * i + 1.0);
        } else {
            term = -1.0 / (2.0 * i + 1.0);
        }
        
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
        
        double new_sum = sum + term;
        if ((term > 0 && new_sum < sum) || (term < 0 && new_sum > sum)) {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
        sum = new_sum;
    }
    
    double pi_approx = 4.0 * sum;
    
    printf("Approximation of pi using %d terms: %.15f\n", n, pi_approx);
    printf("Difference from actual pi: %.15f\n", fabs(pi_approx - 3.141592653589793));
    
    return 0;
}