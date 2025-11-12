//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_TERMS 1000
#define EPSILON 1e-10

double compute_series_sum(int terms) {
    double sum = 0.0;
    double term;
    int i;
    
    if (terms <= 0 || terms > MAX_TERMS) {
        return -1.0;
    }
    
    for (i = 1; i <= terms; i++) {
        term = 1.0 / (i * i);
        if (term < EPSILON) {
            break;
        }
        sum += term;
    }
    
    return sum;
}

double compute_series_sum_while(int terms) {
    double sum = 0.0;
    double term;
    int i = 1;
    
    if (terms <= 0 || terms > MAX_TERMS) {
        return -1.0;
    }
    
    while (i <= terms) {
        term = 1.0 / (i * i);
        if (term < EPSILON) {
            break;
        }
        sum += term;
        i++;
    }
    
    return sum;
}

double compute_series_sum_do_while(int terms) {
    double sum = 0.0;
    double term;
    int i = 1;
    
    if (terms <= 0 || terms > MAX_TERMS) {
        return -1.0;
    }
    
    do {
        term = 1.0 / (i * i);
        if (term < EPSILON) {
            break;
        }
        sum += term;
        i++;
    } while (i <= terms);
    
    return sum;
}

int main(void) {
    int terms;
    double result_for, result_while, result_do_while;
    
    printf("Enter number of terms to compute (1-%d): ", MAX_TERMS);
    if (scanf("%d", &terms) != 1) {
        printf("Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    if (terms <= 0 || terms > MAX_TERMS) {
        printf("Number of terms must be between 1 and %d.\n", MAX_TERMS);
        return 1;
    }
    
    result_for = compute_series_sum(terms);
    result_while = compute_series_sum_while(terms);
    result_do_while = compute_series_sum_do_while(terms);
    
    printf("Series sum using for loop: %.10f\n", result_for);
    printf("Series sum using while loop: %.10f\n", result_while);
    printf("Series sum using do-while loop: %.10f\n", result_do_while);
    
    double pi_approximation = sqrt(6.0 * result_for);
    printf("Approximation of pi: %.10f\n", pi_approximation);
    
    return 0;
}