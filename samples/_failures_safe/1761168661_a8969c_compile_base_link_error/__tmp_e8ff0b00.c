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
    int valid_input;

    printf("Enter first number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    x = strtod(input, NULL);

    printf("Enter operator (+, -, *, /, ^ for power, s for sqrt): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input, " %c", &op) != 1) {
        fprintf(stderr, "Invalid operator\n");
        return 1;
    }

    if (op != 's') {
        printf("Enter second number: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';
        if (!is_valid_double(input)) {
            fprintf(stderr, "Invalid number format\n");
            return 1;
        }
        y = strtod(input, NULL);
    }

    valid_input = 1;
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
            if (y == 0.0) {
                fprintf(stderr, "Division by zero\n");
                valid_input = 0;
            } else {
                result = x / y;
            }
            break;
        case '^':
            if (x == 0.0 && y < 0.0) {
                fprintf(stderr, "Zero to negative power\n");
                valid_input = 0;
            } else if (x < 0.0 && floor(y) != y) {
                fprintf(stderr, "Negative base with fractional exponent\n");
                valid_input = 0;
            } else {
                result = pow(x, y);
            }
            break;
        case 's':
            if (x < 0.0) {
                fprintf(stderr, "Square root of negative number\n");
                valid_input = 0;
            } else {
                result = sqrt(x);
            }
            break;
        default:
            fprintf(stderr, "Unknown operator\n");
            valid_input = 0;
            break;
    }

    if (valid_input) {
        printf("Result: %.6g\n", result);
    }

    return 0;
}