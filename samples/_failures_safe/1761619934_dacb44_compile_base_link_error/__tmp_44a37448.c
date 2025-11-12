//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 50

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    double sequence[ARRAY_SIZE];
    double sum = 0.0;
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        if (term < 0.0 || !isfinite(term)) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return 1;
        }
        sequence[i] = term;
        sum += term;
        i++;
    }
    
    double while_sum = 0.0;
    int j = 0;
    do {
        while_sum += sequence[j];
        j++;
    } while (j < n);
    
    double for_sum = 0.0;
    for (int k = 0; k < n; k++) {
        for_sum += sequence[k];
    }
    
    double pi_estimate = sqrt(6.0 * sum);
    if (!isfinite(pi_estimate)) {
        fprintf(stderr, "Error: Numerical overflow in calculation\n");
        return 1;
    }
    
    printf("Computed %d terms of the Basel problem series\n", n);
    printf("Sum (while loop): %.10f\n", while_sum);
    printf("Sum (for loop): %.10f\n", for_sum);
    printf("Sum (initial): %.10f\n", sum);
    printf("Pi estimate: %.10f\n", pi_estimate);
    printf("Actual Pi: 3.1415926535\n");
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    int idx = 0;
    
    for (int m = 1; m < n; m++) {
        if (sequence[m] > max_val) {
            max_val = sequence[m];
        }
        if (sequence[m] < min_val) {
            min_val = sequence[m];
        }
    }
    
    printf("Maximum term: %.10f\n", max_val);
    printf("Minimum term: %.10f\n", min_val);
    
    return 0;
}