//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        sum += term;
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        double term = 1.0 / (i * i);
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
            double term = 1.0 / (i * i);
            sum += term;
            i++;
        } while (i <= n);
    }
    return sum;
}

int main() {
    int n;
    char input_buffer[100];
    
    printf("Enter number of terms to compute series sum (1-10000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 10000) {
        fprintf(stderr, "Number of terms must be between 1 and 10000\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.15f\n", result_for);
    printf("Series sum using while loop: %.15f\n", result_while);
    printf("Series sum using do-while loop: %.15f\n", result_do_while);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_while - result_do_while);
    
    if (diff1 < TOLERANCE && diff2 < TOLERANCE) {
        printf("All loop variants produced consistent results\n");
    } else {
        printf("Warning: Loop variants produced different results\n");
    }
    
    double pi_approximation = sqrt(6.0 * result_for);
    printf("Approximation of pi: %.15f\n", pi_approximation);
    
    return 0;
}