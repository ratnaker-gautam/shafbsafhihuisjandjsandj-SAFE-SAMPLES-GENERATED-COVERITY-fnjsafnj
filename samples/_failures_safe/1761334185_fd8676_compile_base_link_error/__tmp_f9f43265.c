//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

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
    
    printf("Computing sequence using different loop variants:\n");
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        sequence[i] = term;
        sum += term;
        i++;
    }
    printf("While loop sum: %.10f\n", sum);
    
    sum = 0.0;
    int j;
    for (j = 0; j < n; j++) {
        double term = 1.0 / ((j + 1) * (j + 1));
        sum += term;
    }
    printf("For loop sum: %.10f\n", sum);
    
    sum = 0.0;
    int k = 0;
    do {
        double term = 1.0 / ((k + 1) * (k + 1));
        sum += term;
        k++;
    } while (k < n);
    printf("Do-while loop sum: %.10f\n", sum);
    
    double pi_estimate = sqrt(6.0 * sum);
    printf("Pi estimate (Basel problem): %.10f\n", pi_estimate);
    
    double running_product = 1.0;
    for (int m = 0; m < n; m++) {
        if (m % 2 == 0) {
            running_product *= (1.0 + (1.0 / (m + 1.0)));
        } else {
            running_product *= (1.0 - (1.0 / (m + 1.0)));
        }
        if (running_product > 1e308 || running_product < 1e-308) {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
    }
    printf("Alternating product: %.10f\n", running_product);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    int idx = 0;
    while (idx < n) {
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
        idx++;
    }
    printf("Sequence range: [%.10f, %.10f]\n", min_val, max_val);
    
    return 0;
}