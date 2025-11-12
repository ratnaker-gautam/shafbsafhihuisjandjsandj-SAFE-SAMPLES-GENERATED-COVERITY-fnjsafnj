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
    
    double sequence[ARRAY_SIZE];
    double sum = 0.0;
    
    printf("Computing sequence using different loop variants:\n");
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        sequence[i] = term;
        sum += term;
        i++;
    }
    printf("While loop sum: %.10f\n", sum);
    
    sum = 0.0;
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    printf("For loop sum: %.10f\n", sum);
    
    sum = 0.0;
    int k = 0;
    do {
        sum += sequence[k];
        k++;
    } while (k < n);
    printf("Do-while loop sum: %.10f\n", sum);
    
    sum = 0.0;
    double* ptr = sequence;
    double* end = sequence + n;
    while (ptr < end) {
        sum += *ptr;
        ptr++;
    }
    printf("Pointer arithmetic sum: %.10f\n", sum);
    
    double pi_approximation = sqrt(6.0 * sum);
    printf("Approximation of pi: %.10f\n", pi_approximation);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    for (int idx = 1; idx < n; idx++) {
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
    }
    printf("Sequence range: [%.10f, %.10f]\n", min_val, max_val);
    
    return 0;
}