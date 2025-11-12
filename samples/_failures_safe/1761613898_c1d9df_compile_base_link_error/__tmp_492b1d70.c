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
    
    printf("Computing sequence using different loop styles:\n");
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        sequence[i] = term;
        sum += term;
        i++;
    }
    
    printf("While loop sum: %.10f\n", sum);
    
    sum = 0.0;
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    printf("For loop sum: %.10f\n", sum);
    
    sum = 0.0;
    int k = 0;
    do {
        sum += sequence[k];
        k++;
    } while (k < n);
    
    printf("Do-while loop sum: %.10f\n", sum);
    
    double pi_approximation = sqrt(6.0 * sum);
    printf("Pi approximation: %.10f\n", pi_approximation);
    
    sum = 0.0;
    for (int m = n - 1; m >= 0; m--) {
        sum += sequence[m];
    }
    
    printf("Reverse for loop sum: %.10f\n", sum);
    
    double running_sum = 0.0;
    for (int p = 0; p < n; p++) {
        running_sum += sequence[p];
        if (p % 10 == 9) {
            printf("Running sum after %d terms: %.10f\n", p + 1, running_sum);
        }
    }
    
    return 0;
}