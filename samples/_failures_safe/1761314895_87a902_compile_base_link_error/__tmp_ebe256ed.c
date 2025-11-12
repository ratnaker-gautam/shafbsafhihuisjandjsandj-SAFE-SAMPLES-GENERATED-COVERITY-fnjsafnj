//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

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
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
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
        token = strtok(NULL, " \t\n");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[10];
    int degree;
    char input[MAX_INPUT_LEN];
    double x_value;
    
    printf("Polynomial Calculator\n");
    printf("====================\n\n");
    
    degree = read_coefficients(coefficients, 9);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid coefficients entered\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    
    while (1) {
        printf("\nEnter x value (or 'quit' to exit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        char *endptr;
        x_value = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\0') {
            printf("Error: Invalid number format\n");
            continue;
        }
        
        if (!isfinite(x_value)) {
            printf("Error: Invalid x value\n");
            continue;
        }
        
        double result = compute_polynomial(x_value, coefficients, degree);
        
        if (!isfinite(result)) {
            printf("Error: Computation resulted in invalid value\n");
            continue;
        }
        
        printf("P(%g) = %g\n", x_value, result);
        
        printf("Polynomial terms:\n");
        for (int i = 0; i <= degree; i++) {
            double term = coefficients[i] * pow(x_value, degree - i);
            printf("  %g*x^%d", coefficients[i], degree - i);
            if (i < degree) {
                printf(" + ");
            }
        }
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}