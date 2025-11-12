//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing sum of reciprocals using different loop variants:\n\n");
    
    double sum_while = 0.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        sum_while += 1.0 / i;
        i++;
    }
    printf("While loop result: %.10f\n", sum_while);
    
    double sum_dowhile = 0.0;
    int j = 1;
    if (n >= 1) {
        do {
            if (j == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                return 1;
            }
            sum_dowhile += 1.0 / j;
            j++;
        } while (j <= n);
    }
    printf("Do-while loop result: %.10f\n", sum_dowhile);
    
    double sum_for = 0.0;
    for (int k = 1; k <= n; k++) {
        if (k == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        sum_for += 1.0 / k;
    }
    printf("For loop result: %.10f\n", sum_for);
    
    double sum_goto = 0.0;
    int m = 1;
loop_start:
    if (m <= n) {
        if (m == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        sum_goto += 1.0 / m;
        m++;
        goto loop_start;
    }
    printf("Goto loop result: %.10f\n", sum_goto);
    
    double harmonic_mean = 0.0;
    if (sum_for > 0.0) {
        harmonic_mean = n / sum_for;
    }
    printf("\nHarmonic mean: %.10f\n", harmonic_mean);
    
    double difference = fabs(sum_while - sum_for);
    printf("Maximum difference between methods: %.2e\n", difference);
    
    return 0;
}