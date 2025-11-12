//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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

int parse_coefficients(const char *input, double coefficients[], int max_degree) {
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " ,");
    int count = 0;
    
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) {
            return -1;
        }
        
        coefficients[count] = value;
        count++;
        token = strtok(NULL, " ,");
    }
    
    return count - 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double coefficients[10];
    double x;
    
    printf("Polynomial Calculator\n");
    printf("Enter polynomial coefficients (space/comma separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int degree = parse_coefficients(input, coefficients, 9);
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Invalid coefficients. Enter 1-10 numbers.\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("P(%g) = %g\n", x, result);
    
    printf("Polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%gx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" %c %gx^%d", coefficients[i] >= 0 ? '+' : '-', fabs(coefficients[i]), i);
        } else if (i == 1) {
            printf(" %c %gx", coefficients[i] >= 0 ? '+' : '-', fabs(coefficients[i]));
        } else {
            printf(" %c %g", coefficients[i] >= 0 ? '+' : '-', fabs(coefficients[i]));
        }
    }
    printf("\n");
    
    return 0;
}