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
    
    double running_product = 1.0;
    for (int m = 0; m < n; m++) {
        if (m > 0 && running_product > 1e308 / fabs(sequence[m])) {
            fprintf(stderr, "Warning: Potential overflow in product calculation\n");
            break;
        }
        running_product *= (1.0 + sequence[m]);
    }
    printf("Running product: %.10f\n", running_product);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    for (int p = 1; p < n; p++) {
        if (sequence[p] > max_val) {
            max_val = sequence[p];
        }
        if (sequence[p] < min_val) {
            min_val = sequence[p];
        }
    }
    printf("Maximum value: %.10f\n", max_val);
    printf("Minimum value: %.10f\n", min_val);
    
    return 0;
}