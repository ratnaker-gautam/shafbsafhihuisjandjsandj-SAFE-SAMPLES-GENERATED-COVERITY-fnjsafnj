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
    FACTORIAL,
    EXIT
};

int validate_factorial_input(int n) {
    if (n < 0) return 0;
    if (n > 12) return 0;
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
    enum Operation choice;
    double a, b, result;
    int int_input;
    
    while (1) {
        printf("Available operations:\n");
        printf("1. Add\n");
        printf("2. Subtract\n");
        printf("3. Multiply\n");
        printf("4. Divide\n");
        printf("5. Power\n");
        printf("6. Square Root\n");
        printf("7. Factorial\n");
        printf("8. Exit\n");
        printf("Enter your choice (1-8): ");
        
        if (scanf("%d", (int*)&choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case ADD:
                printf("Enter two numbers: ");
                if (scanf("%lf %lf", &a, &b) != 2) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                result = a + b;
                printf("Result: %.6lf\n", result);
                break;
                
            case SUBTRACT:
                printf("Enter two numbers: ");
                if (scanf("%lf %lf", &a, &b) != 2) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                result = a - b;
                printf("Result: %.6lf\n", result);
                break;
                
            case MULTIPLY:
                printf("Enter two numbers: ");
                if (scanf("%lf %lf", &a, &b) != 2) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                result = a * b;
                printf("Result: %.6lf\n", result);
                break;
                
            case DIVIDE:
                printf("Enter two numbers: ");
                if (scanf("%lf %lf", &a, &b) != 2) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                if (b == 0.0) {
                    printf("Error: Division by zero.\n");
                    break;
                }
                result = a / b;
                printf("Result: %.6lf\n", result);
                break;
                
            case POWER:
                printf("Enter base and exponent: ");
                if (scanf("%lf %lf", &a, &b) != 2) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                if (a < 0.0 && fmod(b, 1.0) != 0.0) {
                    printf("Error: Negative base with fractional exponent.\n");
                    break;
                }
                result = pow(a, b);
                printf("Result: %.6lf\n", result);
                break;
                
            case SQUARE_ROOT:
                printf("Enter a non-negative number: ");
                if (scanf("%lf", &a) != 1) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                if (a < 0.0) {
                    printf("Error: Cannot compute square root of negative number.\n");
                    break;
                }
                result = sqrt(a);
                printf("Result: %.6lf\n", result);
                break;
                
            case FACTORIAL:
                printf("Enter a non-negative integer (0-12): ");
                if (scanf("%d", &int_input) != 1) {
                    printf("Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                if (!validate_factorial_input(int_input)) {
                    printf("Error: Input must be between 0 and 12.\n");
                    break;
                }
                long long fact_result = compute_factorial(int_input);
                printf("Result: %lld\n", fact_result);
                break;
                
            case EXIT:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice. Please select 1-8.\n");
                break;
        }
        printf("\n");
    }
    
    return 0;
}