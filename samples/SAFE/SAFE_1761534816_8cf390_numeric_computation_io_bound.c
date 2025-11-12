//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double calculate_polynomial(double x, double coefficients[], int degree) {
    double result = 0.0;
    for (int i = degree; i >= 0; i--) {
        result = result * x + coefficients[i];
    }
    return result;
}

int is_valid_double(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    int dot_count = 0;
    int digit_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str) {
        if (*str == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (isdigit((unsigned char)*str)) {
            digit_count++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
}

int main() {
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Enter polynomial degree (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (degree < 1 || degree > 10) {
        fprintf(stderr, "Degree must be between 1 and 10\n");
        return 1;
    }
    
    double coefficients[11];
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading coefficient\n");
            return 1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (!is_valid_double(input)) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
        
        if (sscanf(input, "%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Error parsing coefficient\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate polynomial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x value\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double x;
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Error parsing x value\n");
        return 1;
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
    printf("Polynomial: ");
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
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Enter additional x values to evaluate (or 'quit' to exit):\n");
    
    while (1) {
        printf("x = ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (!is_valid_double(input)) {
            printf("Invalid input. Enter a number or 'quit'\n");
            continue;
        }
        
        if (sscanf(input, "%lf", &x) != 1) {
            printf("Error parsing input\n");
            continue;
        }
        
        result = calculate_polynomial(x, coefficients, degree);
        printf("P(%.2f) = %.6f\n", x, result);
    }
    
    return 0;
}