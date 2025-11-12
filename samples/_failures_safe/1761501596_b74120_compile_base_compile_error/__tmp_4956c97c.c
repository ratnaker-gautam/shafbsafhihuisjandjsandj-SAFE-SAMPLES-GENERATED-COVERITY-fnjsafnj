//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    double results[ARRAY_SIZE];
    
    printf("Computing sequence using for loop...\n");
    for (int i = 0; i < n; i++) {
        double term = (i + 1) * (i + 1);
        results[i] = sqrt(term) + sin((double)(i + 1) * 0.1);
    }
    
    printf("Computing running sums using while loop...\n");
    double sum = 0.0;
    int j = 0;
    while (j < n) {
        if (j > 0 && results[j] > 0 && sum > DBL_MAX - results[j]) {
            fprintf(stderr, "Error: Sum would overflow\n");
            return 1;
        }
        sum += results[j];
        j++;
    }
    
    printf("Computing products using do-while loop...\n");
    double product = 1.0;
    int k = 0;
    do {
        if (k >= n) break;
        if (fabs(results[k]) > 0 && fabs(product) > DBL_MAX / fabs(results[k])) {
            fprintf(stderr, "Error: Product would overflow\n");
            return 1;
        }
        product *= results[k];
        k++;
    } while (k < n);
    
    printf("Final sum: %.6f\n", sum);
    printf("Final product: %.6f\n", product);
    
    printf("Computing alternating sum using nested loops...\n");
    double alt_sum = 0.0;
    for (int i = 0; i < n; i++) {
        double inner_sum = 0.0;
        int m = 0;
        while (m <= i && m < 10) {
            inner_sum += (i + m + 1) * 0.01;
            m++;
        }
        if (i % 2 == 0) {
            if (alt_sum > 0 && inner_sum > DBL_MAX - alt_sum) {
                fprintf(stderr, "Error: Sum would overflow\n");
                return 1;
            }
            alt_sum += inner_sum;
        } else {
            if (alt_sum < 0 && -inner_sum < -DBL_MAX - alt_sum) {
                fprintf(stderr, "Error: Sum would overflow\n");
                return 1;
            }
            alt_sum -= inner_sum;
        }
    }
    
    printf("Alternating sum: %.6f\n", alt_sum);
    
    return 0;
}