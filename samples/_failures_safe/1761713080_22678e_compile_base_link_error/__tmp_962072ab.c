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
    
    printf("Computing sequence using different loop styles...\n");
    
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
    
    sum = 0.0;
    int m = 0;
    while (1) {
        if (m >= n) break;
        double term = 1.0 / ((m + 1) * (m + 1));
        sum += term;
        m++;
    }
    
    printf("Break loop sum: %.10f\n", sum);
    
    double pi_estimate = sqrt(6.0 * sum);
    printf("Pi estimate from final sum: %.10f\n", pi_estimate);
    
    double convergence[MAX_ITERATIONS];
    int iter;
    for (iter = 0; iter < MAX_ITERATIONS && iter < n; iter++) {
        convergence[iter] = sqrt(6.0 * sequence[iter] * (iter + 1));
    }
    
    printf("First %d convergence values:\n", (n < 10) ? n : 10);
    for (int idx = 0; idx < ((n < 10) ? n : 10); idx++) {
        printf("Term %d: %.6f\n", idx + 1, convergence[idx]);
    }
    
    return 0;
}