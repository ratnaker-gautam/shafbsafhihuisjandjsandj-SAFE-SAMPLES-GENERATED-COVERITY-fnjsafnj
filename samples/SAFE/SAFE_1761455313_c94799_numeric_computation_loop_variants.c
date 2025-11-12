//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter number of terms to compute (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid number format\n");
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
    
    printf("\nSum (while loop): %.8f\n", sum);
    
    sum = 0.0;
    for (int j = 0; j < n; j++) {
        double term = 1.0 / ((j + 1) * (j + 1));
        sum += term;
    }
    printf("Sum (for loop):   %.8f\n", sum);
    
    sum = 0.0;
    int k = 0;
    do {
        double term = 1.0 / ((k + 1) * (k + 1));
        sum += term;
        k++;
    } while (k < n);
    printf("Sum (do-while):   %.8f\n", sum);
    
    sum = 0.0;
    int m = 0;
    for (;;) {
        if (m >= n) break;
        double term = 1.0 / ((m + 1) * (m + 1));
        sum += term;
        m++;
    }
    printf("Sum (infinite for): %.8f\n", sum);
    
    double expected = 0.0;
    int p = 1;
    while (p <= n) {
        expected += 1.0 / (p * p);
        p++;
    }
    
    printf("Expected sum:     %.8f\n", expected);
    
    if (fabs(sum - expected) < 1e-10) {
        printf("All computations match!\n");
    } else {
        printf("Computation discrepancy detected\n");
    }
    
    return 0;
}