//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main(void) {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double sum = 0.0;
    double pi_approximation;
    
    int i = 0;
    while (i < n) {
        double term = (i % 2 == 0) ? 1.0 : -1.0;
        term /= (2.0 * i + 1.0);
        sum += term;
        i++;
    }
    
    pi_approximation = sum * 4.0;
    
    double alt_sum = 0.0;
    for (int j = 0; j < n; j++) {
        double denominator = (2.0 * j + 1.0);
        if (j % 2 == 0) {
            alt_sum += 1.0 / denominator;
        } else {
            alt_sum -= 1.0 / denominator;
        }
    }
    
    double alt_pi = alt_sum * 4.0;
    
    double third_sum = 0.0;
    int k = 0;
    do {
        double sign = (k % 2 == 0) ? 1.0 : -1.0;
        third_sum += sign / (2.0 * k + 1.0);
        k++;
    } while (k < n);
    
    double third_pi = third_sum * 4.0;
    
    printf("Pi approximations using %d terms:\n", n);
    printf("While loop: %.10f\n", pi_approximation);
    printf("For loop: %.10f\n", alt_pi);
    printf("Do-while loop: %.10f\n", third_pi);
    
    double average = (pi_approximation + alt_pi + third_pi) / 3.0;
    printf("Average: %.10f\n", average);
    
    return 0;
}