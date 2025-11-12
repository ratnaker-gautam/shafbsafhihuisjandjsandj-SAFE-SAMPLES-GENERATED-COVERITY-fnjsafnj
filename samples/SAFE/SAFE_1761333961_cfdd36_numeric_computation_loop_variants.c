//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

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

double compute_series_sum_dowhile(int n) {
    double sum = 0.0;
    int i = 1;
    if (n < 1) return 0.0;
    do {
        if (i == 0) {
            i++;
            continue;
        }
        double term = 1.0 / ((double)i * (double)i);
        sum += term;
        i++;
    } while (i <= n);
    return sum;
}

int main() {
    int n;
    printf("Enter number of terms (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number of terms must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double sum_for = compute_series_sum(n);
    double sum_while = compute_series_sum_while(n);
    double sum_dowhile = compute_series_sum_dowhile(n);
    
    printf("Sum using for loop: %.10f\n", sum_for);
    printf("Sum using while loop: %.10f\n", sum_while);
    printf("Sum using do-while loop: %.10f\n", sum_dowhile);
    
    double diff1 = fabs(sum_for - sum_while);
    double diff2 = fabs(sum_for - sum_dowhile);
    
    if (diff1 < EPSILON && diff2 < EPSILON) {
        printf("All methods produce consistent results\n");
    } else {
        printf("Warning: Results differ between methods\n");
    }
    
    double expected_limit = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Expected limit (pi^2/6): %.10f\n", expected_limit);
    
    return 0;
}