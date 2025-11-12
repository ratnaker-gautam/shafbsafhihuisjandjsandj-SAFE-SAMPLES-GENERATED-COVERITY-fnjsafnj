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
        if (x == HUGE_VAL || x == -HUGE_VAL) {
            printf("Number out of range. Please try again.\n");
            continue;
        }

        printf("Enter operator (+, -, *, /, ^): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) != 1 || strchr("+-*/^", input[0]) == NULL) {
            printf("Invalid operator. Please try again.\n");
            continue;
        }
        op = input[0];

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
        if (y == HUGE_VAL || y == -HUGE_VAL) {
            printf("Number out of range. Please try again.\n");
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
            if (fabs(y) < 1e-15) {
                fprintf(stderr, "Error: Division by zero\n");
                return 1;
            }
            result = x / y;
            break;
        case '^':
            if (x < 0 && fmod(y, 1.0) != 0.0) {
                fprintf(stderr, "Error: Negative base with fractional exponent\n");
                return 1;
            }
            if (x == 0 && y < 0) {
                fprintf(stderr, "Error: Zero to negative power\n");
                return 1;
            }
            result = pow(x, y);
            if (result == HUGE_VAL || result == -HUGE_VAL) {
                fprintf(stderr, "Error: Exponentiation overflow\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Invalid operator\n");
            return 1;
    }

    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Error: Result out of range\n");
        return 1;
    }

    printf("Result: %.15g\n", result);
    return 0;
}