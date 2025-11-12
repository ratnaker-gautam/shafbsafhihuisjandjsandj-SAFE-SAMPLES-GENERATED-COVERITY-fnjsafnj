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
    
    printf("Computing %d terms of alternating harmonic series:\n", n);
    
    double sum = 0.0;
    int i = 0;
    
    while (i < n) {
        double term = 1.0 / (i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        printf("Term %d: %+.6f\n", i + 1, term);
        i++;
    }
    
    printf("\nSum using while loop: %.10f\n", sum);
    
    double sum_for = 0.0;
    for (int j = 0; j < n; j++) {
        double term = 1.0 / (j + 1);
        if (j % 2 == 1) {
            term = -term;
        }
        sum_for += term;
    }
    
    printf("Sum using for loop: %.10f\n", sum_for);
    
    double sum_do = 0.0;
    int k = 0;
    if (n > 0) {
        do {
            double term = 1.0 / (k + 1);
            if (k % 2 == 1) {
                term = -term;
            }
            sum_do += term;
            k++;
        } while (k < n);
    }
    
    printf("Sum using do-while loop: %.10f\n", sum_do);
    
    double expected = log(2.0);
    printf("Expected value (ln 2): %.10f\n", expected);
    
    printf("Differences from expected:\n");
    printf("While loop: %+.2e\n", sum - expected);
    printf("For loop: %+.2e\n", sum_for - expected);
    printf("Do-while loop: %+.2e\n", sum_do - expected);
    
    return 0;
}