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
        if (factorial > 1e308) break;
        result += 1.0 / factorial;
        i++;
    }
    return result;
}

long long factorial(int n) {
    if (n < 0) return -1;
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        if (result > 9223372036854775807LL / i) return -1;
        result *= i;
    }
    return result;
}

int main() {
    int choice;
    printf("Numeric Computation Menu:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Approximate Euler's number\n");
    printf("3. Compute factorial\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            int iterations;
            printf("Enter number of iterations (1-1000000): ");
            if (scanf("%d", &iterations) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (iterations < 1 || iterations > 1000000) {
                printf("Iterations out of range.\n");
                return 1;
            }
            double pi_approx = compute_pi_approximation(iterations);
            printf("Pi approximation: %.10f\n", pi_approx);
            printf("Difference from math.h PI: %.10f\n", fabs(pi_approx - 3.14159265358979323846));
            break;
        }
        
        case 2: {
            int terms;
            printf("Enter number of terms (1-100): ");
            if (scanf("%d", &terms) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (terms < 1 || terms > 100) {
                printf("Terms out of range.\n");
                return 1;
            }
            double e_approx = compute_euler_approximation(terms);
            printf("Euler's number approximation: %.10f\n", e_approx);
            break;
        }
        
        case 3: {
            int number;
            printf("Enter number for factorial (0-20): ");
            if (scanf("%d", &number) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (number < 0 || number > 20) {
                printf("Number out of range.\n");
                return 1;
            }
            long long fact = factorial(number);
            if (fact == -1) {
                printf("Factorial overflow.\n");
            } else {
                printf("%d! = %lld\n", number, fact);
            }
            break;
        }
    }
    
    return 0;
}