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
        if (i + 1 == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        sequence[i] = 1.0 / ((i + 1) * (i + 1));
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    double pi_approximation = sqrt(6.0 * sum);
    printf("Approximation of pi using %d terms: %.10f\n", n, pi_approximation);
    
    double running_sum = 0.0;
    int k = 0;
    do {
        running_sum += sequence[k];
        printf("After term %d: cumulative sum = %.6f\n", k + 1, running_sum);
        k++;
    } while (k < n && k < 10);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    int m;
    for (m = 1; m < n; m++) {
        if (sequence[m] > max_val) {
            max_val = sequence[m];
        }
        if (sequence[m] < min_val) {
            min_val = sequence[m];
        }
    }
    
    printf("Sequence statistics:\n");
    printf("Maximum value: %.10f\n", max_val);
    printf("Minimum value: %.10f\n", min_val);
    printf("Final sum: %.10f\n", sum);
    
    double actual_pi = 3.14159265358979323846;
    double convergence_check = fabs(pi_approximation - actual_pi);
    printf("Error from actual pi: %.10f\n", convergence_check);
    
    return 0;
}