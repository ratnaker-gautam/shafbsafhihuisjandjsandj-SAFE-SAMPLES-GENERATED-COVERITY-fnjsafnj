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
    printf("Term  Value  Partial Sum\n");
    
    double total = 0.0;
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        total += term;
        printf("%4d  %6.4f  %11.6f\n", i + 1, term, total);
        i++;
    }
    
    printf("\nComputing product sequence:\n");
    printf("Term  Value  Partial Product\n");
    
    double product = 1.0;
    int j;
    for (j = 1; j <= n; j++) {
        double factor = 1.0 + (1.0 / j);
        product *= factor;
        printf("%4d  %6.4f  %14.6f\n", j, factor, product);
    }
    
    printf("\nComputing alternating sum:\n");
    printf("Term  Value  Running Sum\n");
    
    double alt_sum = 0.0;
    int k = 1;
    do {
        double alt_term = (k % 2 == 1 ? 1.0 : -1.0) / k;
        alt_sum += alt_term;
        printf("%4d  %6.4f  %11.6f\n", k, alt_term, alt_sum);
        k++;
    } while (k <= n);
    
    printf("\nComputing factorial-like sequence:\n");
    printf("Term  Value\n");
    
    long long fact = 1;
    for (int m = 1; m <= n; m++) {
        if (m > 20) {
            fprintf(stderr, "Factorial overflow protection\n");
            break;
        }
        
        long long prev_fact = fact;
        fact *= m;
        
        if (fact < prev_fact) {
            fprintf(stderr, "Integer overflow detected\n");
            fact = prev_fact;
            break;
        }
        
        printf("%4d  %lld\n", m, fact);
    }
    
    printf("\nFinal results:\n");
    printf("Series sum: %.6f\n", total);
    printf("Product: %.6f\n", product);
    printf("Alternating sum: %.6f\n", alt_sum);
    printf("Last factorial: %lld\n", fact);
    
    return 0;
}