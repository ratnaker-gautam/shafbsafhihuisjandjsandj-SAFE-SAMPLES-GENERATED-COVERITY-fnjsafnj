//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / (i * i);
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        sum += 1.0 / (i * i);
        i++;
    }
    return sum;
}

double compute_series_sum_dowhile(int n) {
    double sum = 0.0;
    int i = 1;
    if (n >= 1) {
        do {
            sum += 1.0 / (i * i);
            i++;
        } while (i <= n);
    }
    return sum;
}

int main(void) {
    char input[32];
    int n;
    
    printf("Enter number of terms to compute series sum (1-10000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 10000) {
        fprintf(stderr, "Number of terms must be between 1 and 10000\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_dowhile = compute_series_sum_dowhile(n);
    
    printf("Series sum using for loop: %.10f\n", result_for);
    printf("Series sum using while loop: %.10f\n", result_while);
    printf("Series sum using do-while loop: %.10f\n", result_dowhile);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_for - result_dowhile);
    
    if (diff1 < 1e-12 && diff2 < 1e-12) {
        printf("All methods produce identical results\n");
    } else {
        printf("Warning: Results differ slightly due to floating-point precision\n");
    }
    
    return 0;
}