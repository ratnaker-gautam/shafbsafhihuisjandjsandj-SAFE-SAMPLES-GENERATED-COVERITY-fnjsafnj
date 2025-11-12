//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    
    for (int i = 0; i < n; i++) {
        results[i] = 0.0;
    }
    
    int i = 0;
    while (i < n) {
        double sum = 0.0;
        int j = 0;
        do {
            if (j >= MAX_ITERATIONS) {
                break;
            }
            double term = 1.0 / ((2.0 * j + 1.0) * (2.0 * j + 1.0));
            if (i % 2 == 0) {
                sum += term;
            } else {
                sum -= term;
            }
            j++;
        } while (j < MAX_ITERATIONS);
        
        results[i] = sum * 8.0;
        i++;
    }
    
    printf("Computed series approximations:\n");
    for (int k = 0; k < n; k++) {
        printf("Term %d: %.10f\n", k + 1, results[k]);
    }
    
    double final_result = 0.0;
    int idx = 0;
    while (idx < n) {
        final_result += results[idx];
        idx++;
    }
    
    printf("Final sum: %.10f\n", final_result);
    printf("Reference PI^2/6: %.10f\n", 3.14159265358979323846 * 3.14159265358979323846 / 6.0);
    
    return 0;
}