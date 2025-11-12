//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_TERMS 1000
#define EPSILON 1e-10

double compute_series_sum(int terms) {
    double sum = 0.0;
    int i;
    
    for (i = 1; i <= terms; i++) {
        double term = 1.0 / (i * i);
        if (term < EPSILON) {
            break;
        }
        sum += term;
    }
    return sum;
}

double compute_alternating_sum(int terms) {
    double sum = 0.0;
    int i = 1;
    
    while (i <= terms) {
        double term = 1.0 / i;
        if (i % 2 == 0) {
            sum -= term;
        } else {
            sum += term;
        }
        i++;
    }
    return sum;
}

double compute_factorial_sum(int terms) {
    double sum = 0.0;
    double factorial = 1.0;
    int i = 1;
    
    do {
        if (i > 1) {
            factorial *= i;
        }
        double term = 1.0 / factorial;
        if (term < EPSILON) {
            break;
        }
        sum += term;
        i++;
    } while (i <= terms);
    
    return sum;
}

int main(void) {
    int choice, terms;
    
    printf("Numeric Series Computation\n");
    printf("1. Sum of 1/n^2 (converges to pi^2/6)\n");
    printf("2. Alternating harmonic series\n");
    printf("3. Sum of 1/n! (converges to e-1)\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Error: Choice must be 1, 2, or 3\n");
        return 1;
    }
    
    printf("Enter number of terms (1-%d): ", MAX_TERMS);
    if (scanf("%d", &terms) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (terms < 1 || terms > MAX_TERMS) {
        printf("Error: Terms must be between 1 and %d\n", MAX_TERMS);
        return 1;
    }
    
    double result;
    
    switch (choice) {
        case 1:
            result = compute_series_sum(terms);
            printf("Sum of first %d terms of 1/n^2: %.10f\n", terms, result);
            printf("Expected limit (pi^2/6): %.10f\n", 3.14159265358979323846 * 3.14159265358979323846 / 6.0);
            break;
        case 2:
            result = compute_alternating_sum(terms);
            printf("Sum of first %d terms of alternating harmonic series: %.10f\n", terms, result);
            printf("Expected limit (ln 2): %.10f\n", log(2.0));
            break;
        case 3:
            result = compute_factorial_sum(terms);
            printf("Sum of first %d terms of 1/n!: %.10f\n", terms, result);
            printf("Expected limit (e-1): %.10f\n", exp(1.0) - 1.0);
            break;
    }
    
    return 0;
}