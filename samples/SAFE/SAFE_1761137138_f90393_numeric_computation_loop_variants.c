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
    
    printf("Computing series sum using different loop variants:\n\n");
    
    double sum_for = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        sum_for += term;
    }
    printf("For loop sum: %.6f\n", sum_for);
    
    double sum_while = 0.0;
    int j = 1;
    while (j <= n) {
        double term = 1.0 / (j * j);
        sum_while += term;
        j++;
    }
    printf("While loop sum: %.6f\n", sum_while);
    
    double sum_do = 0.0;
    int k = 1;
    do {
        if (k > n) break;
        double term = 1.0 / (k * k);
        sum_do += term;
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %.6f\n", sum_do);
    
    double sum_goto = 0.0;
    int m = 1;
    loop_start:
    if (m <= n) {
        double term = 1.0 / (m * m);
        sum_goto += term;
        m++;
        goto loop_start;
    }
    printf("Goto loop sum: %.6f\n", sum_goto);
    
    double expected = 0.0;
    for (int i = 1; i <= n; i++) {
        expected += 1.0 / (i * i);
    }
    
    printf("\nExpected sum: %.6f\n", expected);
    
    double tolerance = 1e-10;
    if (fabs(sum_for - expected) < tolerance &&
        fabs(sum_while - expected) < tolerance &&
        fabs(sum_do - expected) < tolerance &&
        fabs(sum_goto - expected) < tolerance) {
        printf("All loop variants produced correct results\n");
    } else {
        printf("Warning: Loop variants produced different results\n");
    }
    
    return 0;
}