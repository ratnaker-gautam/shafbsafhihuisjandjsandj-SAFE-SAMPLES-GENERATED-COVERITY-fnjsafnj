//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    int n;
    
    printf("Enter number of iterations (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sum = 0.0;
    int i = 0;
    
    while (i < n) {
        double term = 1.0 / ((2.0 * i) + 1.0);
        if (i % 2 == 0) {
            results[i] = term;
        } else {
            results[i] = -term;
        }
        sum += results[i];
        i++;
    }
    
    double pi_approx = 4.0 * sum;
    double actual_pi = 3.14159265358979323846;
    
    printf("Approximating pi using %d terms of Leibniz formula:\n", n);
    
    for (int j = 0; j < n; j++) {
        if (j < 10 || j >= n - 10) {
            printf("Term %d: %f\n", j + 1, results[j]);
        } else if (j == 10) {
            printf("... (showing first and last 10 terms)\n");
        }
    }
    
    printf("Final approximation: %.10f\n", pi_approx);
    printf("Actual pi: %.10f\n", actual_pi);
    printf("Error: %.10f\n", fabs(pi_approx - actual_pi));
    
    double running_sum = 0.0;
    int k = 0;
    do {
        running_sum += results[k];
        k++;
    } while (k < n);
    
    printf("Verification sum: %f\n", running_sum);
    
    return 0;
}