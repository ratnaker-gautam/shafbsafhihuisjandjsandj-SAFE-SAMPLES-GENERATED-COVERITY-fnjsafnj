//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n;
    double x, sum = 0.0, product = 1.0;
    
    printf("Enter number of terms (1-100): ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        printf("Number of terms must be between 1 and 100\n");
        return 1;
    }
    
    printf("Enter base value: ");
    if (scanf("%lf", &x) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (x < -1000.0 || x > 1000.0) {
        printf("Base value must be between -1000 and 1000\n");
        return 1;
    }
    
    for (int i = 1; i <= n; i++) {
        double term;
        
        if (i % 2 == 0) {
            term = pow(x, i) / (double)(i * i);
        } else {
            term = -pow(x, i) / (double)(i * i);
        }
        
        if (!isfinite(term)) {
            printf("Numerical overflow detected at term %d\n", i);
            return 1;
        }
        
        sum += term;
        product *= (1.0 + fabs(term));
        
        if (!isfinite(sum) || !isfinite(product)) {
            printf("Numerical overflow detected\n");
            return 1;
        }
    }
    
    double geometric_mean = pow(product, 1.0 / (double)n);
    
    if (!isfinite(geometric_mean)) {
        printf("Cannot compute geometric mean\n");
        return 1;
    }
    
    printf("Series sum: %.6f\n", sum);
    printf("Product of (1 + |term|): %.6f\n", product);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    return 0;
}