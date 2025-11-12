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

int main() {
    int n;
    printf("Enter number of terms (1-100000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100000) {
        fprintf(stderr, "Error: Number of terms must be between 1 and 100000\n");
        return 1;
    }
    
    double sum_for = compute_series_sum(n);
    double sum_while = compute_series_sum_while(n);
    double sum_do_while = compute_series_sum_do_while(n);
    
    printf("Sum using for loop: %.15f\n", sum_for);
    printf("Sum using while loop: %.15f\n", sum_while);
    printf("Sum using do-while loop: %.15f\n", sum_do_while);
    
    double diff1 = fabs(sum_for - sum_while);
    double diff2 = fabs(sum_for - sum_do_while);
    
    if (diff1 < TOLERANCE && diff2 < TOLERANCE) {
        printf("All methods agree within tolerance\n");
    } else {
        printf("Warning: Methods differ beyond tolerance\n");
    }
    
    double pi_estimate = sqrt(6.0 * sum_for);
    printf("Pi estimate: %.15f\n", pi_estimate);
    printf("Pi actual: 3.141592653589793\n");
    printf("Difference: %.15f\n", fabs(pi_estimate - 3.141592653589793));
    
    return 0;
}