//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter a positive integer (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Number out of range\n");
        return 1;
    }
    
    printf("Computing sum of reciprocals using different loop styles:\n\n");
    
    double sum_while = 0.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) break;
        sum_while += 1.0 / i;
        i++;
    }
    printf("While loop sum: %.6f\n", sum_while);
    
    double sum_for = 0.0;
    for (int j = 1; j <= n; j++) {
        if (j == 0) continue;
        sum_for += 1.0 / j;
    }
    printf("For loop sum: %.6f\n", sum_for);
    
    double sum_do_while = 0.0;
    int k = 1;
    if (k <= n) {
        do {
            if (k == 0) break;
            sum_do_while += 1.0 / k;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %.6f\n", sum_do_while);
    
    double sum_nested = 0.0;
    for (int outer = 1; outer <= n; outer++) {
        double partial_sum = 0.0;
        int inner = 1;
        while (inner <= outer) {
            if (inner == 0) break;
            partial_sum += 1.0 / inner;
            inner++;
        }
        sum_nested += partial_sum / outer;
    }
    printf("Nested loop sum: %.6f\n", sum_nested);
    
    double harmonic_mean = 0.0;
    if (sum_for > 0.0) {
        harmonic_mean = n / sum_for;
    }
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double variance = 0.0;
    double mean = sum_for / n;
    for (int m = 1; m <= n; m++) {
        if (m == 0) continue;
        double diff = (1.0 / m) - mean;
        variance += diff * diff;
    }
    variance /= n;
    printf("Variance of reciprocals: %.6f\n", variance);
    
    return 0;
}