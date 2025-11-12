//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double compute_pi_approximation(int iterations) {
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        double term = 1.0 / (2 * i + 1);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

double compute_euler_approximation(int terms) {
    double result = 1.0;
    double factorial = 1.0;
    int i = 1;
    while (i <= terms) {
        factorial *= i;
        if (factorial == 0.0) {
            break;
        }
        result += 1.0 / factorial;
        i++;
    }
    return result;
}

int main(void) {
    int choice;
    printf("Select computation:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Approximate Euler's number\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        fprintf(stderr, "Error: Invalid choice\n");
        return 1;
    }
    
    int n;
    printf("Enter number of iterations/terms (1-100000): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100000) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double result;
    if (choice == 1) {
        result = compute_pi_approximation(n);
        printf("Pi approximation after %d iterations: %.10f\n", n, result);
        printf("Difference from actual Pi: %.10f\n", fabs(result - 3.1415926535));
    } else {
        result = compute_euler_approximation(n);
        printf("Euler's number approximation after %d terms: %.10f\n", n, result);
        printf("Difference from actual e: %.10f\n", fabs(result - 2.7182818284));
    }
    
    return 0;
}