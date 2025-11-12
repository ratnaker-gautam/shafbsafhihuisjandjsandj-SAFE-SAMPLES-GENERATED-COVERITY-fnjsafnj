//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * power;
        power *= x;
    }
    
    return result;
}

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN + 1];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        return -1;
    }
    
    token = strtok(input, " ");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) {
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[10];
    int degree;
    char input[MAX_INPUT_LEN + 1];
    double x;
    
    degree = read_coefficients(coefficients, 9);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    char *endptr;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' || !isfinite(x)) {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Computation resulted in invalid value\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x, result);
    
    printf("Enter step size for table: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    double step;
    endptr = NULL;
    step = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' || !isfinite(step) || step <= 0) {
        fprintf(stderr, "Error: Invalid step size\n");
        return 1;
    }
    
    printf("Enter number of points: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    long num_points;
    endptr = NULL;
    num_points = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0' || num_points <= 0 || num_points > 100) {
        fprintf(stderr, "Error: Invalid number of points\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation table:\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    double current_x = x;
    for (long i = 0; i < num_points; i++) {
        double value = compute_polynomial(current_x, coefficients, degree);
        if (isfinite(value)) {
            printf("%.6f\t%.6f\n", current_x, value);
        } else {
            printf("%.6f\tNaN\n", current_x);
        }
        current_x += step;
    }
    
    return 0;
}