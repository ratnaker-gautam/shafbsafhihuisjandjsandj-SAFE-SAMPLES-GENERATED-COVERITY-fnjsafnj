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

int main(void) {
    int n;
    char input_buffer[32];
    
    printf("Enter number of terms for series computation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: must be an integer\n");
        return 1;
    }
    
    if (temp <= 0 || temp > MAX_ITERATIONS) {
        fprintf(stderr, "Input must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    n = (int)temp;
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.15f\n", result_for);
    printf("Series sum using while loop: %.15f\n", result_while);
    printf("Series sum using do-while loop: %.15f\n", result_do_while);
    
    double expected = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Expected value (π²/6): %.15f\n", expected);
    
    double diff_for = fabs(result_for - expected);
    double diff_while = fabs(result_while - expected);
    double diff_do_while = fabs(result_do_while - expected);
    
    printf("Difference from expected (for): %.2e\n", diff_for);
    printf("Difference from expected (while): %.2e\n", diff_while);
    printf("Difference from expected (do-while): %.2e\n", diff_do_while);
    
    if (diff_for < TOLERANCE && diff_while < TOLERANCE && diff_do_while < TOLERANCE) {
        printf("All loop variants produced accurate results\n");
    } else {
        printf("Some results may be inaccurate\n");
    }
    
    return 0;
}