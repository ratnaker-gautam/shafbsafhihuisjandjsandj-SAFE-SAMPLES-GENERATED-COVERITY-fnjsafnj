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
    char *token;
    int count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    token = strtok(input, " ");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
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
    double x_values[100];
    int degree, num_points;
    
    printf("Enter polynomial degree (0-9): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 9) {
        printf("Degree must be between 0 and 9\n");
        return 1;
    }
    
    printf("Enter %d coefficients (space separated): ", degree + 1);
    getchar();
    
    int coeff_count = read_double_array(coefficients, degree + 1);
    if (coeff_count != degree + 1) {
        printf("Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter number of x values to evaluate (1-100): ");
    if (scanf("%d", &num_points) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_points < 1 || num_points > 100) {
        printf("Number of points must be between 1 and 100\n");
        return 1;
    }
    
    printf("Enter %d x values (space separated): ", num_points);
    getchar();
    
    int x_count = read_double_array(x_values, num_points);
    if (x_count != num_points) {
        printf("Invalid x values\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation results:\n");
    printf("x\t\tf(x)\n");
    printf("----------------\n");
    
    for (int i = 0; i < num_points; i++) {
        double result = calculate_polynomial(x_values[i], coefficients, degree);
        printf("%.6f\t%.6f\n", x_values[i], result);
    }
    
    return 0;
}