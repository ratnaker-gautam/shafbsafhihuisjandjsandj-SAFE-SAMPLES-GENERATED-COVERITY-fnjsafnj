//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double (*numeric_operation)(double, double);

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (fabs(b) < 1e-12) {
        return NAN;
    }
    return a / b;
}

double power(double a, double b) {
    if (a < 0 && fmod(b, 1.0) != 0) {
        return NAN;
    }
    return pow(a, b);
}

double math_op1(double x, double y) {
    return sin(x) + cos(y);
}

double math_op2(double x, double y) {
    if (fabs(x) < 0) return NAN;
    double term1 = sqrt(fabs(x));
    if (fabs(y) + 1 <= 0) return NAN;
    double term2 = log(fabs(y) + 1);
    return term1 + term2;
}

double math_op3(double x, double y) {
    return exp(-fabs(x - y));
}

int get_valid_double(const char* prompt, double* value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

void perform_operation(numeric_operation op, const char* op_name, double a, double b) {
    double result = op(a, b);
    if (isnan(result)) {
        printf("Operation %s failed: invalid input\n", op_name);
    } else {
        printf("%s result: %.6f\n", op_name, result);
    }
}

int main() {
    double a, b;
    
    printf("Numeric Computation with Function Pointers\n");
    printf("==========================================\n");
    
    if (!get_valid_double("Enter first number: ", &a)) {
        printf("Invalid input for first number\n");
        return 1;
    }
    
    if (!get_valid_double("Enter second number: ", &b)) {
        printf("Invalid input for second number\n");
        return 1;
    }
    
    printf("\nPerforming operations on %.6f and %.6f:\n", a, b);
    
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < num_operations; i++) {
        perform_operation(operations[i], operation_names[i], a, b);
    }
    
    printf("\nComputing mathematical properties:\n");
    
    numeric_operation math_ops[] = {math_op1, math_op2, math_op3};
    const char* math_names[] = {"sin(a) + cos(b)", "sqrt(|a|) + ln(|b|+1)", "exp(-|a-b|)"};
    int num_math_ops = sizeof(math_ops) / sizeof(math_ops[0]);
    
    for (int i = 0; i < num_math_ops; i++) {
        double result = math_ops[i](a, b);
        if (isnan(result) || isinf(result)) {
            printf("%s: undefined\n", math_names[i]);
        } else {
            printf("%s: %.6f\n", math_names[i], result);
        }
    }
    
    return 0;
}