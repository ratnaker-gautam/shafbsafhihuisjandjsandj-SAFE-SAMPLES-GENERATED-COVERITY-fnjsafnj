//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_polynomial(double x, double coeffs[], int degree) {
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
    
    printf("Enter polynomial degree (0-9): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 9) {
        printf("Degree must be between 0 and 9\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Enter %d coefficients (space separated): ", degree + 1);
    int coeff_count = read_double_array(coefficients, degree + 1);
    
    if (coeff_count != degree + 1) {
        printf("Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x values to evaluate (space separated, max 10): ");
    double x_values[10];
    int x_count = read_double_array(x_values, 10);
    
    if (x_count <= 0) {
        printf("No valid x values provided\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("x\t\tf(x)\n");
    printf("--\t\t----\n");
    
    for (int i = 0; i < x_count; i++) {
        double result = calculate_polynomial(x_values[i], coefficients, degree);
        if (isnan(result) || isinf(result)) {
            printf("%.6f\t\tOverflow/NaN\n", x_values[i]);
        } else {
            printf("%.6f\t\t%.6f\n", x_values[i], result);
        }
    }
    
    return 0;
}