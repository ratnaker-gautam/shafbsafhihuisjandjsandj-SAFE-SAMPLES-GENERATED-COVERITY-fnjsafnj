//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT];
    int n;
    
    printf("Enter number of terms to compute (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 20) {
        fprintf(stderr, "Number must be between 1 and 20\n");
        return 1;
    }
    
    printf("Computing series sum using different loop variants:\n\n");
    
    double sum_for = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        sum_for += term;
    }
    printf("For loop sum: %.6f\n", sum_for);
    
    double sum_while = 0.0;
    int j = 1;
    while (j <= n) {
        double term = 1.0 / (j * j);
        sum_while += term;
        j++;
    }
    printf("While loop sum: %.6f\n", sum_while);
    
    double sum_do_while = 0.0;
    int k = 1;
    do {
        if (n >= 1) {
            double term = 1.0 / (k * k);
            sum_do_while += term;
        }
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %.6f\n", sum_do_while);
    
    double sum_nested = 0.0;
    for (int i = 1; i <= n; i++) {
        double inner_sum = 0.0;
        int m = 1;
        while (m <= i) {
            inner_sum += 1.0 / (m * m);
            m++;
        }
        sum_nested += inner_sum / i;
    }
    printf("Nested loop sum: %.6f\n", sum_nested);
    
    double pi_approximation = sqrt(6.0 * sum_for);
    printf("Pi approximation: %.6f\n", pi_approximation);
    
    return 0;
}