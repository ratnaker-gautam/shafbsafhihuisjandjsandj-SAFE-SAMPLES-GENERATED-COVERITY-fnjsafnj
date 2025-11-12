//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_OPERATOR,
    CALCULATING,
    ERROR,
    DONE
};

struct Calculator {
    enum State state;
    double current_value;
    double operand;
    char operator;
    int has_operand;
};

void init_calculator(struct Calculator *calc) {
    calc->state = INIT;
    calc->current_value = 0.0;
    calc->operand = 0.0;
    calc->operator = '\0';
    calc->has_operand = 0;
}

int is_valid_operator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}

double perform_operation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b != 0.0) return a / b;
            return 0.0;
        default: return 0.0;
    }
}

int process_input(struct Calculator *calc, const char *input) {
    size_t len = strlen(input);
    if (len == 0) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (calc->state) {
            case INIT:
                if (isdigit(c)) {
                    calc->current_value = c - '0';
                    calc->state = READING_NUMBER;
                } else if (c == 'q') {
                    calc->state = DONE;
                    return 1;
                } else if (!isspace(c)) {
                    calc->state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    calc->current_value = calc->current_value * 10 + (c - '0');
                } else if (is_valid_operator(c)) {
                    calc->operator = c;
                    calc->operand = calc->current_value;
                    calc->has_operand = 1;
                    calc->state = READING_OPERATOR;
                } else if (c == '=') {
                    calc->state = DONE;
                    return 1;
                } else if (!isspace(c)) {
                    calc->state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isdigit(c)) {
                    calc->current_value = c - '0';
                    calc->state = CALCULATING;
                } else if (!isspace(c)) {
                    calc->state = ERROR;
                }
                break;
                
            case CALCULATING:
                if (isdigit(c)) {
                    calc->current_value = calc->current_value * 10 + (c - '0');
                } else if (is_valid_operator(c)) {
                    if (calc->has_operand) {
                        calc->current_value = perform_operation(calc->operand, calc->current_value, calc->operator);
                    }
                    calc->operator = c;
                    calc->operand = calc->current_value;
                    calc->has_operand = 1;
                    calc->state = READING_OPERATOR;
                } else if (c == '=') {
                    if (calc->has_operand) {
                        calc->current_value = perform_operation(calc->operand, calc->current_value, calc->operator);
                    }
                    calc->state = DONE;
                    return 1;
                } else if (!isspace(c)) {
                    calc->state = ERROR;
                }
                break;
                
            case ERROR:
                return 0;
                
            case DONE:
                return 1;
        }
    }
    
    return 0;
}

int main(void) {
    struct Calculator calc;
    char input[256];
    
    init_calculator(&calc);
    
    printf("Simple Calculator (enter 'q' to quit, '=' to get result)\n");
    printf("Supported operations: + - * /\n");
    printf("Enter expression: ");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        if (process_input(&calc, input)) {
            if (calc.state == DONE) {
                printf("Result: %.2f\n", calc.current_value);
                break;
            } else if (calc.state == ERROR) {
                printf("Error: Invalid expression\n");
                break;
            }
        }
        
        if (calc.state == DONE || calc.state == ERROR) {
            break;
        }
        
        printf("Enter next part: ");
    }
    
    return 0;
}