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
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    double a, b, result;
    int op_choice, int_input;
    char buffer[100];
    
    printf("Available operations:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Divide\n4: Power\n5: Square Root\n6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &op_choice) != 1) {
        fprintf(stderr, "Invalid operation selection\n");
        return 1;
    }
    
    if (op_choice < ADD || op_choice > FACTORIAL) {
        fprintf(stderr, "Operation out of range\n");
        return 1;
    }
    
    switch (op_choice) {
        case ADD:
        case SUBTRACT:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            printf("Enter first number: ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (sscanf(buffer, "%lf", &a) != 1) {
                fprintf(stderr, "Invalid number\n");
                return 1;
            }
            
            printf("Enter second number: ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (sscanf(buffer, "%lf", &b) != 1) {
                fprintf(stderr, "Invalid number\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case FACTORIAL:
            printf("Enter number: ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (sscanf(buffer, "%lf", &a) != 1) {
                fprintf(stderr, "Invalid number\n");
                return 1;
            }
            break;
    }
    
    switch (op_choice) {
        case ADD:
            result = a + b;
            printf("Result: %.6lf\n", result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("Result: %.6lf\n", result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("Result: %.6lf\n", result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-15) {
                fprintf(stderr, "Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6lf\n", result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                fprintf(stderr, "Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.6lf\n", result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                fprintf(stderr, "Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6lf\n", result);
            break;
            
        case FACTORIAL:
            if (a != floor(a)) {
                fprintf(stderr, "Factorial requires integer\n");
                return 1;
            }
            int_input = (int)a;
            if (!validate_factorial(int_input)) {
                fprintf(stderr, "Factorial input out of range (0-12)\n");
                return 1;
            }
            long long fact_result = compute_factorial(int_input);
            printf("Result: %lld\n", fact_result);
            break;
            
        default:
            fprintf(stderr, "Invalid operation\n");
            return 1;
    }
    
    return 0;
}