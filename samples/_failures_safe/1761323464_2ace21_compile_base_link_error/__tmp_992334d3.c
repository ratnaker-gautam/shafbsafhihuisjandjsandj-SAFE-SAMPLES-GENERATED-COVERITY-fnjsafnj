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
    if (n < 1) return 0.0;
    do {
        double term = 1.0 / (i * i);
        sum += term;
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
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input: must be an integer\n");
        return 1;
    }
    
    if (val < 1 || val > 1000000) {
        fprintf(stderr, "Invalid input: must be between 1 and 1000000\n");
        return 1;
    }
    
    n = (int)val;
    if (n != val) {
        fprintf(stderr, "Input value too large\n");
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
    
    double pi_approx = sqrt(6.0 * result_for);
    printf("Approximation of pi: %.15f\n", pi_approx);
    
    return 0;
}