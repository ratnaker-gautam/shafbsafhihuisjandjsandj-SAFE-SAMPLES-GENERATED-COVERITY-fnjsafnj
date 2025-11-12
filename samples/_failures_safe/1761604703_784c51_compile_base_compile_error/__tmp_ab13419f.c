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
    INVALID
};

int validate_integer_input(const char* input, long* result) {
    char* endptr;
    *result = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    if (*result < -1000000 || *result > 1000000) {
        return 0;
    }
    return 1;
}

int validate_double_input(const char* input, double* result) {
    char* endptr;
    *result = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    if (*result < -1e6 || *result > 1e6) {
        return 0;
    }
    return 1;
}

long compute_factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long result = 1;
    for (int i = 2; i <= n; i++) {
        if (result > LONG_MAX / i) {
            return -1;
        }
        result *= i;
    }
    return result;
}

int main() {
    char input[256];
    enum Operation op = INVALID;
    double num1 = 0, num2 = 0;
    long long_num = 0;
    int valid_input = 0;

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

    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' || choice < 0 || choice > 6) {
        fprintf(stderr, "Invalid operation selection\n");
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
            if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (!validate_double_input(input, &num1)) {
                fprintf(stderr, "Invalid first number\n");
                return 1;
            }

            printf("Enter second number: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (!validate_double_input(input, &num2)) {
                fprintf(stderr, "Invalid second number\n");
                return 1;
            }
            break;

        case SQUARE_ROOT:
            printf("Enter number: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (!validate_double_input(input, &num1)) {
                fprintf(stderr, "Invalid number\n");
                return 1;
            }
            break;

        case FACTORIAL:
            printf("Enter integer (0-20): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
            if (!validate_integer_input(input, &long_num)) {
                fprintf(stderr, "Invalid integer\n");
                return 1;
            }
            if (long_num < 0 || long_num > 20) {
                fprintf(stderr, "Integer must be between 0 and 20\n");
                return 1;
            }
            break;

        default:
            fprintf(stderr, "Invalid operation\n");
            return 1;
    }

    double result = 0;
    long fact_result = 0;

    switch (op) {
        case ADD:
            result = num1 + num2;
            printf("Result: %.6f\n", result);
            break;

        case SUBTRACT:
            result = num1 - num2;
            printf("Result: %.6f\n", result);
            break;

        case MULTIPLY:
            result = num1 * num2;
            printf("Result: %.6f\n", result);
            break;

        case DIVIDE:
            if (fabs(num2) < 1e-12) {
                fprintf(stderr, "Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            printf("Result: %.6f\n", result);
            break;

        case POWER:
            if (num1 == 0 && num2 < 0) {
                fprintf(stderr, "Zero to negative power undefined\n");
                return 1;
            }
            result = pow(num1, num2);
            printf("Result: %.6f\n", result