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
    if (n > 0) {
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
    
    printf("Enter number of terms for series computation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (temp <= 0 || temp > 1000000) {
        fprintf(stderr, "Invalid input: must be between 1 and 1000000\n");
        return 1;
    }
    
    n = (int)temp;
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.15f\n", result_for);
    printf("Series sum using while loop: %.15f\n", result_while);
    printf("Series sum using do-while loop: %.15f\n", result_do_while);
    
    double pi_squared_over_6 = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Theoretical limit (π²/6): %.15f\n", pi_squared_over_6);
    
    double convergence_error = fabs(result_for - pi_squared_over_6);
    printf("Convergence error: %.15f\n", convergence_error);
    
    return 0;
}