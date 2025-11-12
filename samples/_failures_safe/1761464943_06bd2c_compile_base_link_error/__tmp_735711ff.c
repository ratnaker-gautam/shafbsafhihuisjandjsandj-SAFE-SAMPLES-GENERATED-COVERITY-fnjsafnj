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

int main() {
    enum Operation op;
    double a, b, result;
    int choice, valid;
    unsigned long long fact_input;
    unsigned long long factorial_result;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Enter operation number: ");
    
    valid = scanf("%d", &choice);
    if (valid != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 6) {
        printf("Invalid operation\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    
    switch (op) {
        case ADD:
            printf("Enter two numbers: ");
            valid = scanf("%lf %lf", &a, &b);
            if (valid != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a + b;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            valid = scanf("%lf %lf", &a, &b);
            if (valid != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a - b;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            valid = scanf("%lf %lf", &a, &b);
            if (valid != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a * b;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            valid = scanf("%lf %lf", &a, &b);
            if (valid != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (b == 0.0) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            printf("Enter base and exponent: ");
            valid = scanf("%lf %lf", &a, &b);
            if (valid != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0.0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            valid = scanf("%lf", &a);
            if (valid != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0.0) {
                printf("Error: Cannot calculate square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer (0-20): ");
            valid = scanf("%llu", &fact_input);
            if (valid != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (fact_input > 20) {
                printf("Error: Input too large for factorial calculation\n");
                return 1;
            }
            factorial_result = 1;
            for (unsigned long long i = 2; i <= fact_input; i++) {
                factorial_result *= i;
            }
            printf("Result: %llu\n", factorial_result);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}