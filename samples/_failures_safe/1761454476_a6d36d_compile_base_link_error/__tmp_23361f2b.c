//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int n) {
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * pow(x, n - 1 - i);
    }
    return result;
}

int read_double_array(double arr[], int max_size) {
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
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
    
    printf("Enter polynomial degree (1-9): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (degree < 1 || degree > 9) {
        printf("Degree must be between 1 and 9\n");
        return 1;
    }
    
    printf("Enter %d coefficients separated by spaces: ", degree + 1);
    getchar();
    
    int num_coeffs = read_double_array(coefficients, degree + 1);
    if (num_coeffs != degree + 1) {
        printf("Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x values to evaluate (one per line, 'q' to quit):\n");
    
    char line[MAX_INPUT_LEN];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == 'q' && (line[1] == '\n' || line[1] == '\0')) {
            break;
        }
        
        char *endptr;
        double x = strtod(line, &endptr);
        if (endptr == line || (*endptr != '\0' && *endptr != '\n')) {
            printf("Invalid input\n");
            continue;
        }
        if (x == HUGE_VAL || x == -HUGE_VAL) {
            printf("Value out of range\n");
            continue;
        }
        
        double result = compute_polynomial(x, coefficients, degree + 1);
        if (result == HUGE_VAL || result == -HUGE_VAL) {
            printf("Result out of range\n");
            continue;
        }
        
        printf("P(%.6f) = %.6f\n", x, result);
    }
    
    return 0;
}