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

int main() {
    int n;
    char input_buffer[100];
    
    printf("Enter number of terms for series computation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n <= 0) {
        fprintf(stderr, "Number of terms must be positive\n");
        return 1;
    }
    
    if (n > MAX_ITERATIONS) {
        fprintf(stderr, "Number of terms exceeds maximum allowed\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.10f\n", result_for);
    printf("Series sum using while loop: %.10f\n", result_while);
    printf("Series sum using do-while loop: %.10f\n", result_do_while);
    
    double reference = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    double partial_sum = compute_series_sum(n);
    double error = fabs(partial_sum - reference);
    
    printf("Reference value (pi^2/6): %.10f\n", reference);
    printf("Error for %d terms: %.10f\n", n, error);
    
    return 0;
}