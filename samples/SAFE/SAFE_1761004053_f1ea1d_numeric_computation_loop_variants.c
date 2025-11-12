//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
    
    double sum_while = 0.0;
    int i = 1;
    printf("While loop: ");
    while (i <= n) {
        double term = 1.0 / (i * i);
        sum_while += term;
        printf("%.6f", term);
        if (i < n) printf(" + ");
        i++;
    }
    printf(" = %.6f\n", sum_while);
    
    double sum_for = 0.0;
    printf("For loop: ");
    for (int j = 1; j <= n; j++) {
        double term = 1.0 / (j * j);
        sum_for += term;
        printf("%.6f", term);
        if (j < n) printf(" + ");
    }
    printf(" = %.6f\n", sum_for);
    
    double sum_do_while = 0.0;
    int k = 1;
    printf("Do-while loop: ");
    do {
        double term = 1.0 / (k * k);
        sum_do_while += term;
        printf("%.6f", term);
        if (k < n) printf(" + ");
        k++;
    } while (k <= n);
    printf(" = %.6f\n", sum_do_while);
    
    printf("\nVerification - all sums should be equal:\n");
    printf("While loop sum: %.10f\n", sum_while);
    printf("For loop sum: %.10f\n", sum_for);
    printf("Do-while loop sum: %.10f\n", sum_do_while);
    
    double diff1 = fabs(sum_while - sum_for);
    double diff2 = fabs(sum_for - sum_do_while);
    
    if (diff1 < 1e-10 && diff2 < 1e-10) {
        printf("All loop variants produced identical results.\n");
    } else {
        printf("Warning: Loop variants produced different results.\n");
    }
    
    return 0;
}