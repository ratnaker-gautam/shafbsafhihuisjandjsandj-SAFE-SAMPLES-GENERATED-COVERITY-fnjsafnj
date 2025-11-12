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
    for (int i = 1; i <= terms; i++) {
        factorial *= i;
        result += 1.0 / factorial;
    }
    return result;
}

int main(void) {
    int choice;
    printf("Select computation:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Approximate Euler's number\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    int iterations;
    printf("Enter number of iterations (1-100000): ");
    if (scanf("%d", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (iterations < 1 || iterations > 100000) {
        printf("Iterations out of range\n");
        return 1;
    }
    
    double result;
    if (choice == 1) {
        result = compute_pi_approximation(iterations);
        printf("Pi approximation: %.10f\n", result);
        printf("Actual Pi: 3.1415926535\n");
        printf("Difference: %.10f\n", fabs(result - 3.1415926535));
    } else {
        result = compute_euler_approximation(iterations);
        printf("Euler's number approximation: %.10f\n", result);
        printf("Actual e: 2.7182818284\n");
        printf("Difference: %.10f\n", fabs(result - 2.7182818284));
    }
    
    printf("\nComputing convergence...\n");
    double convergence[10];
    int step = iterations / 10;
    if (step < 1) step = 1;
    
    int count = 0;
    int n = step;
    while (n <= iterations && count < 10) {
        double approx;
        if (choice == 1) {
            approx = compute_pi_approximation(n);
        } else {
            approx = compute_euler_approximation(n);
        }
        convergence[count] = approx;
        printf("At %d iterations: %.10f\n", n, approx);
        n += step;
        count++;
    }
    
    return 0;
}