//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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

int get_valid_double(const char* prompt, double* value) {
    char buffer[256];
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    
    return 1;
}

void perform_operation(numeric_operation op, const char* op_name, double a, double b) {
    double result = op(a, b);
    
    if (isnan(result)) {
        printf("Operation '%s' on %.2f and %.2f produced an invalid result\n", op_name, a, b);
    } else if (isinf(result)) {
        printf("Operation '%s' on %.2f and %.2f produced infinity\n", op_name, a, b);
    } else {
        printf("%s: %.2f %s %.2f = %.6f\n", op_name, a, op_name, b, result);
    }
}

int main() {
    double num1, num2;
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation with Function Pointers\n");
    printf("==========================================\n");
    
    if (!get_valid_double("Enter first number: ", &num1)) {
        printf("Error: Invalid input for first number\n");
        return 1;
    }
    
    if (!get_valid_double("Enter second number: ", &num2)) {
        printf("Error: Invalid input for second number\n");
        return 1;
    }
    
    printf("\nPerforming operations on %.2f and %.2f:\n", num1, num2);
    printf("----------------------------------------\n");
    
    for (int i = 0; i < num_operations; i++) {
        perform_operation(operations[i], operation_names[i], num1, num2);
    }
    
    printf("\nComputing mathematical properties:\n");
    printf("----------------------------------\n");
    
    double sum = add(num1, num2);
    double product = multiply(num1, num2);
    double avg = divide(sum, 2.0);
    
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Average: %.6f\n", avg);
    
    if (num1 >= 0 && num2 >= 0) {
        double geo_mean = sqrt(product);
        printf("Geometric mean: %.6f\n", geo_mean);
    }
    
    return 0;
}