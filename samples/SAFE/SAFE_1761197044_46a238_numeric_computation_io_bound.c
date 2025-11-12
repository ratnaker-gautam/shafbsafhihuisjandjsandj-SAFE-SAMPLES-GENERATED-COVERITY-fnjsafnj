//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int read_double_array(double arr[], int max_size) {
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        arr[count] = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    int degree;
    double x_start, x_end, step;
    int num_points;
    
    printf("Enter polynomial degree (0-9): ");
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Degree must be between 0 and 9\n");
        return 1;
    }
    
    printf("Enter %d coefficients (space-separated): ", degree + 1);
    getchar();
    
    int coeff_count = read_double_array(coefficients, degree + 1);
    if (coeff_count != degree + 1) {
        fprintf(stderr, "Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x range (start end): ");
    if (scanf("%lf %lf", &x_start, &x_end) != 2) {
        fprintf(stderr, "Invalid range input\n");
        return 1;
    }
    
    if (x_start > x_end) {
        fprintf(stderr, "Start must be <= end\n");
        return 1;
    }
    
    printf("Enter number of points: ");
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Invalid points input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > 1000) {
        fprintf(stderr, "Number of points must be between 2 and 1000\n");
        return 1;
    }
    
    step = (x_end - x_start) / (num_points - 1);
    
    printf("\nPolynomial evaluation results:\n");
    printf("x\t\ty\n");
    printf("----------------\n");
    
    for (int i = 0; i < num_points; i++) {
        double x = x_start + i * step;
        double y = compute_polynomial(x, coefficients, degree);
        
        if (!isfinite(y)) {
            fprintf(stderr, "Numerical overflow at x = %f\n", x);
            return 1;
        }
        
        printf("%.6f\t%.6f\n", x, y);
    }
    
    return 0;
}