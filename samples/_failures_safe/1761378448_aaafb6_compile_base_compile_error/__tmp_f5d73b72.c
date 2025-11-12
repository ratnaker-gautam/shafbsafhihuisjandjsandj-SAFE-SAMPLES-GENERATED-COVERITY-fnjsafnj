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
    FACTORIAL,
    GCD
};

int validate_positive_int(int n) {
    return n >= 0;
}

int validate_nonzero(int n) {
    return n != 0;
}

int compute_factorial(int n) {
    if (n == 0) return 1;
    int result = 1;
    for (int i = 1; i <= n; i++) {
        if (result > INT_MAX / i) {
            return -1;
        }
        result *= i;
    }
    return result;
}

int compute_gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    enum Operation op;
    int choice;
    double num1, num2, result;
    int int1, int2, int_result;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("7: GCD\n");
    printf("Select operation (0-7): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 7) {
        printf("Invalid operation\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    
    switch (op) {
        case ADD:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &num1, &num2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = num1 + num2;
            printf("Result: %.2f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &num1, &num2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = num1 - num2;
            printf("Result: %.2f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &num1, &num2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = num1 * num2;
            printf("Result: %.2f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &num1, &num2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (fabs(num2) < 1e-10) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            printf("Result: %.2f\n", result);
            break;
            
        case POWER:
            printf("Enter base and exponent: ");
            if (scanf("%lf %lf", &num1, &num2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (num1 < 0 && fmod(num2, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(num1, num2);
            printf("Result: %.2f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            if (scanf("%lf", &num1) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(num1);
            printf("Result: %.2f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer: ");
            if (scanf("%d", &int1) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (!validate_positive_int(int1)) {
                printf("Error: Input must be non-negative\n");
                return 1;
            }
            int_result = compute_factorial(int1);
            if (int_result == -1) {
                printf("Error: Factorial too large\n");
                return 1;
            }
            printf("Result: %d\n", int_result);
            break;
            
        case GCD:
            printf("Enter two integers: ");
            if (scanf("%d %d", &int1, &int2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (int1 == 0 && int2 == 0) {
                printf("Error: Both numbers cannot be zero\n");
                return 1;
            }
            int_result = compute_gcd(int1, int2);
            printf("Result: %d\n", int_result);