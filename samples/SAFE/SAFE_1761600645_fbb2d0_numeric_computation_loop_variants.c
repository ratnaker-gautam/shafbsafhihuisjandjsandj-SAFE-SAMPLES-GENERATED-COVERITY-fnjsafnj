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
    
    printf("Approximating pi using %d terms\n", n);
    printf("Result: %.10f\n", pi_approx);
    
    double running_sum = 0.0;
    printf("Partial sums:\n");
    for (int j = 0; j < n; j++) {
        running_sum += results[j];
        double partial_pi = 4.0 * running_sum;
        printf("Term %d: %.10f\n", j + 1, partial_pi);
    }
    
    double squared_sum = 0.0;
    int k = 0;
    do {
        squared_sum += results[k] * results[k];
        k++;
    } while (k < n);
    
    printf("Sum of squared terms: %.10f\n", squared_sum);
    
    double max_val = results[0];
    double min_val = results[0];
    for (int m = 1; m < n; m++) {
        if (results[m] > max_val) {
            max_val = results[m];
        }
        if (results[m] < min_val) {
            min_val = results[m];
        }
    }
    
    printf("Maximum term: %.10f\n", max_val);
    printf("Minimum term: %.10f\n", min_val);
    
    return 0;
}