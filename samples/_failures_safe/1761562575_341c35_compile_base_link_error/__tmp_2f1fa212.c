//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQUARE_ROOT,
    FACTORIAL
};

int validate_factorial(int n) {
    if (n < 0 || n > 12) {
        return 0;
    }
    return 1;
}

long long compute_factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    enum Operation op;
    double a, b, result;
    int int_val;
    char input[100];
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &int_val) != 1) {
        fprintf(stderr, "Invalid operation selection\n");
        return 1;
    }
    
    if (int_val < 0 || int_val > 6) {
        fprintf(stderr, "Operation out of range\n");
        return 1;
    }
    
    op = (enum Operation)int_val;
    
    switch (op) {
        case ADD:
            printf("Enter two numbers: ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%lf %lf", &a, &b) != 2) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            result = a + b;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%lf %lf", &a, &b) != 2) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            result = a - b;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%lf %lf", &a, &b) != 2) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            result = a * b;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%lf %lf", &a, &b) != 2) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            if (fabs(b) < 1e-15) {
                fprintf(stderr, "Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            printf("Enter base and exponent: ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%lf %lf", &a, &b) != 2) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                fprintf(stderr, "Invalid exponent for negative base\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%lf", &a) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            if (a < 0) {
                fprintf(stderr, "Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter an integer (0-12): ");
            if (fgets(input, sizeof(input), stdin) == NULL ||
                sscanf(input, "%d", &int_val) != 1) {
                fprintf(stderr, "Invalid input\n");
                return 1;
            }
            if (!validate_factorial(int_val)) {
                fprintf(stderr, "Factorial input out of range\n");
                return 1;
            }
            long long fact_result = compute_factorial(int_val);
            printf("Result: %lld\n", fact_result);
            break;
            
        default:
            fprintf(stderr, "Unknown operation\n");
            return 1;
    }
    
    return 0;
}