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
    printf("For loop sum: %.10f\n", sum_for);
    
    double sum_while = 0.0;
    int j = 1;
    while (j <= n) {
        double term = 1.0 / (j * j);
        sum_while += term;
        j++;
    }
    printf("While loop sum: %.10f\n", sum_while);
    
    double sum_do_while = 0.0;
    int k = 1;
    if (n >= 1) {
        do {
            double term = 1.0 / (k * k);
            sum_do_while += term;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %.10f\n", sum_do_while);
    
    double sum_goto = 0.0;
    int m = 1;
    loop_start:
    if (m <= n) {
        double term = 1.0 / (m * m);
        sum_goto += term;
        m++;
        goto loop_start;
    }
    printf("Goto loop sum: %.10f\n", sum_goto);
    
    double reference = 0.0;
    for (int i = 1; i <= n; i++) {
        reference += 1.0 / (i * i);
    }
    
    double tolerance = 1e-12;
    if (fabs(sum_for - reference) < tolerance &&
        fabs(sum_while - reference) < tolerance &&
        fabs(sum_do_while - reference) < tolerance &&
        fabs(sum_goto - reference) < tolerance) {
        printf("\nAll loop variants produced consistent results.\n");
    } else {
        printf("\nWarning: Loop variants produced different results.\n");
    }
    
    return 0;
}