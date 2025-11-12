//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Operation {
    ADD = 1,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQUARE_ROOT,
    FACTORIAL
};

double compute_factorial(int n) {
    if (n < 0) return -1.0;
    double result = 1.0;
    for (int i = 1; i <= n; i++) {
        result *= i;
        if (result > 1e308) return -1.0;
    }
    return result;
}

int main(void) {
    int choice;
    double a, b, result;
    
    printf("Numeric Computation Tool\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Divide\n");
    printf("5. Power\n");
    printf("6. Square Root\n");
    printf("7. Factorial\n");
    printf("Select operation (1-7): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (choice < ADD || choice > FACTORIAL) {
        printf("Error: Invalid operation\n");
        return 1;
    }
    
    switch (choice) {
        case ADD:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Error: Invalid input\n");
                return 1;
            }
            result = a + b;
            printf("Result: %.6lf\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Error: Invalid input\n");
                return 1;
            }
            result = a - b;
            printf("Result: %.6lf\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Error: Invalid input\n");
                return 1;
            }
            result = a * b;
            printf("Result: %.6lf\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Error: Invalid input\n");
                return 1;
            }
            if (fabs(b) < 1e-15) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6lf\n", result);
            break;
            
        case POWER:
            printf("Enter base and exponent: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Error: Invalid input\n");
                return 1;
            }
            if (a < 0.0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            if (isnan(result) || isinf(result)) {
                printf("Error: Invalid power operation\n");
                return 1;
            }
            printf("Result: %.6lf\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Error: Invalid input\n");
                return 1;
            }
            if (a < 0.0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6lf\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer: ");
            if (scanf("%lf", &a) != 1) {
                printf("Error: Invalid input\n");
                return 1;
            }
            if (a < 0.0 || fmod(a, 1.0) != 0.0) {
                printf("Error: Input must be non-negative integer\n");
                return 1;
            }
            result = compute_factorial((int)a);
            if (result < 0.0) {
                printf("Error: Factorial too large or invalid input\n");
                return 1;
            }
            printf("Result: %.0lf\n", result);
            break;
            
        default:
            printf("Error: Invalid operation\n");
            return 1;
    }
    
    return 0;
}