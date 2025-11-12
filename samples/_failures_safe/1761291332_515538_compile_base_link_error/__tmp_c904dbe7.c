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
    enum Operation op;
    double a, b, result;
    int int_input, op_choice;
    
    printf("Available operations:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Divide\n4: Power\n5: Square Root\n6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 6) {
        printf("Invalid operation\n");
        return 1;
    }
    
    op = (enum Operation)op_choice;
    
    switch (op) {
        case ADD:
        case SUBTRACT:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            printf("Enter first number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case FACTORIAL:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            break;
    }
    
    switch (op) {
        case ADD:
            result = a + b;
            printf("Result: %.2lf\n", result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("Result: %.2lf\n", result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("Result: %.2lf\n", result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.2lf\n", result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.2lf\n", result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.2lf\n", result);
            break;
            
        case FACTORIAL:
            if (fmod(a, 1.0) != 0 || a < 0) {
                printf("Error: Factorial requires non-negative integer\n");
                return 1;
            }
            int_input = (int)a;
            if (!validate_factorial(int_input)) {
                printf("Error: Factorial input must be between 0 and 12\n");
                return 1;
            }
            long long fact_result = compute_factorial(int_input);
            printf("Result: %lld\n", fact_result);
            break;
    }
    
    return 0;
}