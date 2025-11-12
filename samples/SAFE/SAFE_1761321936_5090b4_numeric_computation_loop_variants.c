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
    
    double sum_for = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        sum_for += 1.0 / i;
    }
    printf("For loop sum: %.10f\n", sum_for);
    
    double sum_while = 0.0;
    int j = 1;
    while (j <= n) {
        if (j == 0) {
            j++;
            continue;
        }
        sum_while += 1.0 / j;
        j++;
    }
    printf("While loop sum: %.10f\n", sum_while);
    
    double sum_do_while = 0.0;
    int k = 1;
    if (n >= 1) {
        do {
            if (k == 0) {
                k++;
                if (k > n) break;
                continue;
            }
            sum_do_while += 1.0 / k;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %.10f\n", sum_do_while);
    
    double sum_nested = 0.0;
    for (int outer = 1; outer <= n; outer++) {
        int iterations = (outer < MAX_ITERATIONS) ? outer : MAX_ITERATIONS;
        double partial_sum = 0.0;
        for (int inner = 1; inner <= iterations; inner++) {
            if (inner == 0) continue;
            partial_sum += 1.0 / (inner * outer);
        }
        sum_nested += partial_sum;
    }
    printf("Nested loop sum: %.10f\n", sum_nested);
    
    double sum_break_continue = 0.0;
    for (int m = 1; m <= n; m++) {
        if (m % 100 == 0 && m > 100) {
            continue;
        }
        if (m > MAX_ITERATIONS) {
            break;
        }
        if (m == 0) continue;
        sum_break_continue += 1.0 / m;
    }
    printf("Break/continue loop sum: %.10f\n", sum_break_continue);
    
    double difference = fabs(sum_for - sum_while);
    printf("\nMaximum difference between methods: %.10f\n", difference);
    
    return 0;
}