//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define M_PI 3.14159265358979323846

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        double term = 1.0 / ((double)i * (double)i);
        sum += term;
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
        sum += term;
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    double sum = 0.0;
    int i = 1;
    if (n >= 1) {
        do {
            if (i == 0) {
                i++;
                continue;
            }
            double term = 1.0 / ((double)i * (double)i);
            sum += term;
            i++;
        } while (i <= n);
    }
    return sum;
}

int main(void) {
    int n;
    char input_buffer[100];
    
    printf("Enter number of terms to compute (1-100000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 100000) {
        fprintf(stderr, "Number of terms must be between 1 and 100000\n");
        return 1;
    }
    
    printf("Computing sum of 1/k^2 from k=1 to k=%d\n", n);
    
    double sum_for = compute_series_sum(n);
    double sum_while = compute_series_sum_while(n);
    double sum_do_while = compute_series_sum_do_while(n);
    
    printf("Result using for loop: %.15f\n", sum_for);
    printf("Result using while loop: %.15f\n", sum_while);
    printf("Result using do-while loop: %.15f\n", sum_do_while);
    
    if (fabs(sum_for - sum_while) < TOLERANCE && fabs(sum_for - sum_do_while) < TOLERANCE) {
        printf("All methods agree within tolerance\n");
    } else {
        printf("Warning: Methods produced different results\n");
    }
    
    double pi_estimate = sqrt(6.0 * sum_for);
    printf("Pi estimate from series: %.15f\n", pi_estimate);
    printf("Actual Pi: %.15f\n", M_PI);
    printf("Error: %.15f\n", fabs(pi_estimate - M_PI));
    
    return 0;
}