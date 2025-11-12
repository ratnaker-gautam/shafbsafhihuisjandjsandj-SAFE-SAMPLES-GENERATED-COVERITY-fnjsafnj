//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100

double calculate_polynomial(double x, double coefficients[], int degree) {
    double result = 0.0;
    double power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * power;
        power *= x;
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
    
    token = strtok(input, " ,");
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
        token = strtok(NULL, " ,");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    double x_values[5];
    int degree, num_points;
    
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
        printf("Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter number of x values to evaluate (1-5): ");
    if (scanf("%d", &num_points) != 1) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    if (num_points < 1 || num_points > 5) {
        printf("Number of points must be between 1 and 5\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Enter %d x values (space separated): ", num_points);
    int x_count = read_double_array(x_values, num_points);
    if (x_count != num_points) {
        printf("Invalid x values\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation results:\n");
    printf("P(x) = ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2fx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.2fx^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2f", coefficients[i]);
        }
    }
    printf("\n\n");
    
    for (int i = 0; i < num_points; i++) {
        double result = calculate_polynomial(x_values[i], coefficients, degree);
        printf("P(%.2f) = %.6f\n", x_values[i], result);
    }
    
    return 0;
}