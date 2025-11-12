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
    
    printf("Computing series sum for %d terms:\n", n);
    printf("Term  Value\n");
    
    double sum = 0.0;
    int i = 0;
    
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        sum += term;
        printf("%4d  %.6f\n", i + 1, term);
        i++;
    }
    
    printf("\n");
    printf("Direct sum: %.10f\n", sum);
    
    double reverse_sum = 0.0;
    for (int j = n; j > 0; j--) {
        reverse_sum += 1.0 / (j * j);
    }
    printf("Reverse sum: %.10f\n", reverse_sum);
    
    double kahan_sum = 0.0;
    double compensation = 0.0;
    int k = 0;
    do {
        double term = 1.0 / ((k + 1) * (k + 1));
        double y = term - compensation;
        double t = kahan_sum + y;
        compensation = (t - kahan_sum) - y;
        kahan_sum = t;
        k++;
    } while (k < n);
    printf("Kahan sum: %.10f\n", kahan_sum);
    
    double pairwise_sum = 0.0;
    int m = n;
    while (m > 1) {
        for (int p = 0; p < m - 1; p += 2) {
            double term1 = 1.0 / ((p + 1) * (p + 1));
            double term2 = 1.0 / ((p + 2) * (p + 2));
            pairwise_sum += term1 + term2;
        }
        if (m % 2 == 1) {
            pairwise_sum += 1.0 / (m * m);
        }
        m = (m + 1) / 2;
    }
    if (n == 1) {
        pairwise_sum = 1.0;
    }
    printf("Pairwise sum: %.10f\n", pairwise_sum);
    
    double expected = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    double partial_sum = 0.0;
    for (int q = 1; q <= n; q++) {
        partial_sum += 1.0 / (q * q);
    }
    printf("Expected limit: %.10f\n", expected);
    printf("Difference: %.10f\n", fabs(expected - partial_sum));
    
    return 0;
}