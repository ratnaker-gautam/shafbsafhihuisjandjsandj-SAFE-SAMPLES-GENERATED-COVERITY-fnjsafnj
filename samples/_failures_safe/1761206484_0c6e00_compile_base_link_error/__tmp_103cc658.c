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
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

int read_double_array(double arr[], int max_size) {
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *token = strtok(input, " ");
    int count = 0;
    
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0' && *endptr != '\n' && *endptr != ' ') {
            return -1;
        }
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            return -1;
        }
        arr[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    int degree;
    double x_value;
    
    printf("Enter polynomial degree (0-9): ");
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Degree must be between 0 and 9\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Enter %d coefficients (space separated): ", degree + 1);
    int coeff_count = read_double_array(coefficients, degree + 1);
    if (coeff_count != degree + 1) {
        fprintf(stderr, "Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "X value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Result out of range\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    printf("Evaluating polynomial at multiple points...\n");
    printf("Enter number of evaluation points: ");
    
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Invalid number of points\n");
        return 1;
    }
    
    if (num_points < 1 || num_points > 100) {
        fprintf(stderr, "Number of points must be between 1 and 100\n");
        return 1;
    }
    
    for (int i = 0; i < num_points; i++) {
        printf("Enter point %d: ", i + 1);
        double point;
        if (scanf("%lf", &point) != 1) {
            fprintf(stderr, "Invalid point\n");
            return 1;
        }
        
        if (point == HUGE_VAL || point == -HUGE_VAL) {
            fprintf(stderr, "Point out of range\n");
            return 1;
        }
        
        double eval_result = compute_polynomial(point, coefficients, degree);
        
        if (eval_result == HUGE_VAL || eval_result == -HUGE_VAL) {
            fprintf(stderr, "Evaluation result out of range\n");
            return 1;
        }
        
        printf("P(%.6f) = %.6f\n", point, eval_result);
    }
    
    return 0;
}