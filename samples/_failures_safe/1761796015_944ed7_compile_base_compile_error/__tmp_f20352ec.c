//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

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
    if (n > 0) {
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
    }
    return sum;
}

int main(void) {
    int n;
    char input_buffer[32];
    
    printf("Enter a positive integer (max %d): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n <= 0) {
        fprintf(stderr, "Input must be positive\n");
        return 1;
    }
    
    if (n > MAX_INPUT) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    printf("Computing sum of 1/k^2 from k=1 to %d using different loops:\n", n);
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("For loop result: %.15f\n", result_for);
    printf("While loop result: %.15f\n", result_while);
    printf("Do-while loop result: %.15f\n", result_do_while);
    
    if (!isfinite(result_for)) {
        fprintf(stderr, "Numerical overflow in computation\n");
        return 1;
    }
    
    double pi_approximation = sqrt(6.0 * result_for);
    if (!isfinite(pi_approximation)) {
        fprintf(stderr, "Numerical overflow in pi approximation\n");
        return 1;
    }
    
    printf("Approximation of pi: %.15f\n", pi_approximation);
    
    double actual_pi = 3.14159265358979323846;
    double error = fabs(pi_approximation - actual_pi);
    printf("Error from actual pi: %.15f\n", error);
    
    return 0;
}