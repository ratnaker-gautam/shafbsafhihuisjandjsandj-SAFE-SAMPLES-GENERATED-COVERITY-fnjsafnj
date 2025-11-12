//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    double x, y, result;
    char op;
    int valid_input = 0;

    while (!valid_input) {
        printf("Enter first number: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';
        if (!is_valid_double(input)) {
            printf("Invalid number. Please try again.\n");
            continue;
        }
        x = strtod(input, NULL);

        printf("Enter operator (+, -, *, /, ^): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        if (strlen(input) != 2 || input[1] != '\n') {
            printf("Invalid operator. Please try again.\n");
            continue;
        }
        op = input[0];
        if (op != '+' && op != '-' && op != '*' && op != '/' && op != '^') {
            printf("Invalid operator. Please try again.\n");
            continue;
        }

        printf("Enter second number: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';
        if (!is_valid_double(input)) {
            printf("Invalid number. Please try again.\n");
            continue;
        }
        y = strtod(input, NULL);

        if (op == '/' && y == 0.0) {
            printf("Division by zero is not allowed. Please try again.\n");
            continue;
        }

        if (op == '^' && x == 0.0 && y < 0.0) {
            printf("Zero to negative power is undefined. Please try again.\n");
            continue;
        }

        valid_input = 1;
    }

    switch (op) {
        case '+':
            result = x + y;
            break;
        case '-':
            result = x - y;
            break;
        case '*':
            result = x * y;
            break;
        case '/':
            result = x / y;
            break;
        case '^':
            result = pow(x, y);
            break;
        default:
            fprintf(stderr, "Unexpected error\n");
            return 1;
    }

    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }

    printf("Result: %.6f\n", result);
    return 0;
}