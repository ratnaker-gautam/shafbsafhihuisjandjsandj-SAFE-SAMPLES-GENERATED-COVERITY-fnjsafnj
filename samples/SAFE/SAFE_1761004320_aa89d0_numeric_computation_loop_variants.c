//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
    
    double sum_for = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        sum_for += 1.0 / i;
    }
    printf("For loop sum: %.6f\n", sum_for);
    
    double sum_while = 0.0;
    int j = 1;
    while (j <= n) {
        if (j != 0) {
            sum_while += 1.0 / j;
        }
        j++;
    }
    printf("While loop sum: %.6f\n", sum_while);
    
    double sum_do_while = 0.0;
    int k = 1;
    if (n >= 1) {
        do {
            if (k != 0) {
                sum_do_while += 1.0 / k;
            }
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %.6f\n", sum_do_while);
    
    double sum_nested = 0.0;
    for (int outer = 1; outer <= n; outer++) {
        double partial_sum = 0.0;
        int inner = 1;
        while (inner <= outer) {
            if (inner != 0) {
                partial_sum += 1.0 / inner;
            }
            inner++;
        }
        sum_nested += partial_sum;
    }
    printf("Nested loop sum: %.6f\n", sum_nested);
    
    double sum_break = 0.0;
    for (int m = 1; m <= n * 2; m++) {
        if (m > n) {
            break;
        }
        if (m != 0) {
            sum_break += 1.0 / m;
        }
    }
    printf("Break loop sum: %.6f\n", sum_break);
    
    double sum_continue = 0.0;
    for (int p = 0; p <= n; p++) {
        if (p == 0) {
            continue;
        }
        sum_continue += 1.0 / p;
    }
    printf("Continue loop sum: %.6f\n", sum_continue);
    
    return 0;
}