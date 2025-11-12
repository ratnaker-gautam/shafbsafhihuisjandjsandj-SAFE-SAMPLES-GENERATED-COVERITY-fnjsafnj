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
        double term = sequence[j] * (j + 1);
        sum += term;
    }
    printf("For loop sum: %.10f\n", sum);
    
    sum = 0.0;
    int k = 0;
    do {
        double term = sqrt(sequence[k]);
        sum += term;
        k++;
    } while (k < n);
    printf("Do-while loop sum: %.10f\n", sum);
    
    sum = 0.0;
    int m;
    for (m = n - 1; m >= 0; m--) {
        double term = sequence[m] * cos(m * 0.1);
        if (isnan(term) || isinf(term)) {
            term = 0.0;
        }
        sum += term;
    }
    printf("Reverse for loop sum: %.10f\n", sum);
    
    double final_result = 0.0;
    int p, q;
    for (p = 0; p < n; p++) {
        double inner_sum = 0.0;
        for (q = 0; q <= p; q++) {
            if (q < ARRAY_SIZE) {
                inner_sum += sequence[q];
            }
        }
        final_result += inner_sum / (p + 1);
    }
    printf("Final computed value: %.10f\n", final_result);
    
    return 0;
}