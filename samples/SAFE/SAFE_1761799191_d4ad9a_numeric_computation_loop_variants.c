//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define PI 3.14159265358979323846

double compute_pi_approximation(int method) {
    double sum = 0.0;
    
    if (method == 1) {
        for (int k = 0; k < MAX_ITERATIONS; k++) {
            double sign = (k % 2 == 0) ? 1.0 : -1.0;
            double denominator = 2.0 * k + 1.0;
            if (denominator == 0.0) {
                return -1.0;
            }
            double term = sign / denominator;
            sum += term;
        }
        return 4.0 * sum;
    } else if (method == 2) {
        int k = 0;
        while (k < MAX_ITERATIONS) {
            double sign = (k % 2 == 0) ? 1.0 : -1.0;
            double denominator = 2.0 * k + 1.0;
            if (denominator == 0.0) {
                return -1.0;
            }
            double term = sign / denominator;
            sum += term;
            k++;
        }
        return 4.0 * sum;
    } else if (method == 3) {
        int k = 0;
        do {
            double sign = (k % 2 == 0) ? 1.0 : -1.0;
            double denominator = 2.0 * k + 1.0;
            if (denominator == 0.0) {
                return -1.0;
            }
            double term = sign / denominator;
            sum += term;
            k++;
        } while (k < MAX_ITERATIONS);
        return 4.0 * sum;
    } else {
        return -1.0;
    }
}

int main(void) {
    int method;
    char input_buffer[100];
    
    printf("Select approximation method (1=for, 2=while, 3=do-while): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &method) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (method < 1 || method > 3) {
        fprintf(stderr, "Method must be 1, 2, or 3\n");
        return 1;
    }
    
    double result = compute_pi_approximation(method);
    
    if (result < 0) {
        fprintf(stderr, "Computation error\n");
        return 1;
    }
    
    printf("Approximated pi: %.10f\n", result);
    printf("Actual pi: %.10f\n", PI);
    printf("Absolute error: %.10f\n", fabs(result - PI));
    
    return 0;
}