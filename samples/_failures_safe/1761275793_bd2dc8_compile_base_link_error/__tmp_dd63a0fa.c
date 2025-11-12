//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    double sum = 0.0;
    double product = 1.0;
    
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing sum and product using different loop variants...\n");
    
    int i = 1;
    while (i <= n) {
        sum += i;
        product *= i;
        if (isinf(product) || isnan(product)) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        i++;
    }
    
    printf("While loop results:\n");
    printf("Sum: %.0f\n", sum);
    printf("Product: %.0f\n", product);
    
    sum = 0.0;
    product = 1.0;
    
    for (int j = 1; j <= n; j++) {
        sum += j;
        if (j > 1) {
            product *= j;
            if (isinf(product) || isnan(product)) {
                fprintf(stderr, "Error: Product overflow\n");
                return 1;
            }
        }
    }
    
    printf("For loop results:\n");
    printf("Sum: %.0f\n", sum);
    printf("Product: %.0f\n", product);
    
    sum = 0.0;
    product = 1.0;
    int k = 1;
    
    do {
        sum += k;
        product *= k;
        if (isinf(product) || isnan(product)) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        k++;
    } while (k <= n);
    
    printf("Do-while loop results:\n");
    printf("Sum: %.0f\n", sum);
    printf("Product: %.0f\n", product);
    
    double harmonic_sum = 0.0;
    double geometric_sum = 0.0;
    double ratio = 0.5;
    
    for (int m = 1; m <= n && m <= MAX_ITERATIONS; m++) {
        harmonic_sum += 1.0 / m;
        geometric_sum += pow(ratio, m - 1);
        
        if (isinf(harmonic_sum) || isnan(harmonic_sum) || 
            isinf(geometric_sum) || isnan(geometric_sum)) {
            fprintf(stderr, "Error: Numerical overflow in series\n");
            return 1;
        }
    }
    
    printf("Series computations:\n");
    printf("Harmonic sum (first %d terms): %f\n", (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS, harmonic_sum);
    printf("Geometric sum (first %d terms): %f\n", (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS, geometric_sum);
    
    return 0;
}