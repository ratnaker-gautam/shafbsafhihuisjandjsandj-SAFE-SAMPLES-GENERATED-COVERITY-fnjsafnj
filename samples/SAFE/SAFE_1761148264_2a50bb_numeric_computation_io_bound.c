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
    for (int i = degree; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

int read_double_array(double arr[], int max_size) {
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *token = strtok(input, " \t\n");
    int count = 0;
    
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        if (value < -1e100 || value > 1e100) {
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
    
    printf("Enter polynomial degree (0-9): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (degree < 0 || degree > 9) {
        printf("Degree must be between 0 and 9\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Enter %d coefficients (from constant to highest degree): ", degree + 1);
    int num_coeffs = read_double_array(coefficients, degree + 1);
    if (num_coeffs != degree + 1) {
        printf("Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x values to evaluate (one per line, empty line to stop):\n");
    
    char line[MAX_INPUT_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        
        char *endptr;
        double x = strtod(line, &endptr);
        if (endptr == line || (*endptr != '\0' && *endptr != '\n')) {
            printf("Invalid x value\n");
            continue;
        }
        
        if (x < -1e100 || x > 1e100) {
            printf("X value out of range\n");
            continue;
        }
        
        double result = compute_polynomial(x, coefficients, degree);
        if (isnan(result) || isinf(result)) {
            printf("x=%.6f: computation error\n", x);
        } else {
            printf("x=%.6f: f(x)=%.6f\n", x, result);
        }
        
        count++;
        if (count >= 100) {
            break;
        }
    }
    
    if (count == 0) {
        printf("No valid x values provided\n");
    }
    
    return 0;
}