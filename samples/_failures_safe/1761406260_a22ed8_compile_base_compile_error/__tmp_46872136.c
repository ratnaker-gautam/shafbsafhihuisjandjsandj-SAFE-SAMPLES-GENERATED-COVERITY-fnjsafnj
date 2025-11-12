//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        double term = 1.0 / ((double)i * (double)i);
        if (!isfinite(term)) continue;
        sum += term;
        if (!isfinite(sum)) break;
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            i++;
            continue;
        }
        double term = 1.0 / ((double)i * (double)i);
        if (!isfinite(term)) {
            i++;
            continue;
        }
        sum += term;
        if (!isfinite(sum)) break;
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    double sum = 0.0;
    int i = 1;
    if (n <= 0) return 0.0;
    do {
        if (i == 0) {
            i++;
            continue;
        }
        double term = 1.0 / ((double)i * (double)i);
        if (!isfinite(term)) {
            i++;
            continue;
        }
        sum += term;
        if (!isfinite(sum)) break;
        i++;
    } while (i <= n);
    return sum;
}

int main(void) {
    char input[32];
    int n;
    
    printf("Enter number of terms for series computation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n <= 0) {
        fprintf(stderr, "Number of terms must be positive\n");
        return 1;
    }
    
    if (n > MAX_ITERATIONS) {
        fprintf(stderr, "Number of terms exceeds maximum allowed (%d)\n", MAX_ITERATIONS);
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.15f\n", result_for);
    printf("Series sum using while loop: %.15f\n", result_while);
    printf("Series sum using do-while loop: %.15f\n", result_do_while);
    
    if (!isfinite(result_for)) {
        fprintf(stderr, "Numerical overflow in computation\n");
        return 1;
    }
    
    double pi_estimate = sqrt(6.0 * result_for);
    if (!isfinite(pi_estimate)) {
        fprintf(stderr, "Numerical overflow in pi estimation\n");
        return 1;
    }
    
    printf("Estimated value of pi: %.15f\n", pi_estimate);
    
    double actual_pi = 3.14159265358979323846;
    double error = fabs(pi_estimate - actual_pi);
    printf("Error from actual pi: %.15f\n", error);
    
    return 0;
}