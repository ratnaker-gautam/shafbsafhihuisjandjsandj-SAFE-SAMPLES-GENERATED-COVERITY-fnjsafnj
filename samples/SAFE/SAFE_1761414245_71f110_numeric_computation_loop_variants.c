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
        if (term < TOLERANCE) break;
        sum += term;
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        double term = 1.0 / (i * i);
        if (term < TOLERANCE) break;
        sum += term;
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    double sum = 0.0;
    int i = 1;
    if (n <= 0) return sum;
    do {
        double term = 1.0 / (i * i);
        if (term < TOLERANCE) break;
        sum += term;
        i++;
    } while (i <= n);
    return sum;
}

int main(void) {
    char input[32];
    int n;
    
    printf("Enter number of terms to compute (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("For loop result: %.10f\n", result_for);
    printf("While loop result: %.10f\n", result_while);
    printf("Do-while loop result: %.10f\n", result_do_while);
    
    double pi_squared_over_6 = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Theoretical limit (π²/6): %.10f\n", pi_squared_over_6);
    
    return 0;
}