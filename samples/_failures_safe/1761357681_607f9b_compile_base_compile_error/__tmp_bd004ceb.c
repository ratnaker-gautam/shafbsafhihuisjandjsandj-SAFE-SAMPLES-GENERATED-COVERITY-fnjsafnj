//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

enum Operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQUARE_ROOT,
    FACTORIAL
};

long factorial(int n) {
    if (n < 0) return -1;
    long result = 1;
    for (int i = 1; i <= n; i++) {
        if (result > LONG_MAX / i) return -1;
        result *= i;
    }
    return result;
}

int main() {
    enum Operation op;
    int choice;
    double a, b, result;
    int int_val;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (scanf("%d", &choice) != 1) {
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
            printf("%.2f + %.2f = %.2f\n", a, b, result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("%.2f - %.2f = %.2f\n", a, b, result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("%.2f * %.2f = %.2f\n", a, b, result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-10) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("%.2f / %.2f = %.2f\n", a, b, result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            if (a == 0 && b < 0) {
                printf("Error: Zero to negative power\n");
                return 1;
            }
            result = pow(a, b);
            printf("%.2f ^ %.2f = %.2f\n", a, b, result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("sqrt(%.2f) = %.2f\n", a, result);
            break;
            
        case FACTORIAL:
            if (a < 0 || a > 20 || fmod(a, 1.0) != 0.0) {
                printf("Error: Factorial requires integer between 0 and 20\n");
                return 1;
            }
            int_val = (int)a;
            long fact_result = factorial(int_val);
            if (fact_result == -1) {
                printf("Error: Factorial overflow\n");
                return 1;
            }
            printf("%d! = %ld\n", int_val, fact_result);
            break;
    }
    
    return 0;
}