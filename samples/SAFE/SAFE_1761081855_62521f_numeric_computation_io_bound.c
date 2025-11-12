//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double calculate_polynomial(double x, double coefficients[], int degree) {
    double result = 0.0;
    double power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * power;
        power *= x;
    }
    
    return result;
}

int read_coefficients(double coefficients[], int max_degree) {
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients from highest to lowest degree (space separated): ");
    
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
        
        coefficients[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[20];
    int degree;
    char input[MAX_INPUT_LEN];
    double x;
    
    printf("Polynomial Evaluator\n");
    printf("===================\n\n");
    
    degree = read_coefficients(coefficients, 19);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    
    while (1) {
        printf("\nEnter x value to evaluate (or 'quit' to exit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char *endptr;
        x = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\0') {
            printf("Error: Invalid number format\n");
            continue;
        }
        
        if (!isfinite(x)) {
            printf("Error: Invalid floating point value\n");
            continue;
        }
        
        double result = calculate_polynomial(x, coefficients, degree);
        
        if (!isfinite(result)) {
            printf("Error: Result is not finite\n");
            continue;
        }
        
        printf("P(%.6f) = %.6f\n", x, result);
        
        printf("Polynomial: ");
        for (int i = degree; i >= 0; i--) {
            if (i == degree) {
                printf("%.2fx^%d", coefficients[i], i);
            } else if (i > 1) {
                printf(" %+.2fx^%d", coefficients[i], i);
            } else if (i == 1) {
                printf(" %+.2fx", coefficients[i]);
            } else {
                printf(" %+.2f", coefficients[i]);
            }
        }
        printf("\n");
    }
    
    return 0;
}