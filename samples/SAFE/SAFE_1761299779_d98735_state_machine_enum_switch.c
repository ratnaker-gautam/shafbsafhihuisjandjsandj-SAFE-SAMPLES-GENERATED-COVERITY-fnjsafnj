//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
            else return 0.0;
        default: return 0.0;
    }
}

int process_input(struct Calculator *calc, const char *input) {
    char *endptr;
    double number;
    
    switch (calc->state) {
        case INIT:
        case READING_OPERATOR:
            number = strtod(input, &endptr);
            if (endptr != input && *endptr == '\0') {
                if (calc->state == INIT) {
                    calc->current_value = number;
                } else {
                    calc->operand = number;
                    calc->has_operand = 1;
                }
                calc->state = READING_NUMBER;
                return 1;
            }
            calc->state = ERROR;
            return 0;
            
        case READING_NUMBER:
            if (strlen(input) == 1 && is_valid_operator(input[0])) {
                calc->operator = input[0];
                calc->state = READING_OPERATOR;
                return 1;
            } else if (strcmp(input, "=") == 0) {
                calc->state = DONE;
                return 1;
            }
            calc->state = ERROR;
            return 0;
            
        case CALCULATING:
            if (calc->has_operand) {
                calc->current_value = perform_operation(calc->current_value, calc->operand, calc->operator);
                calc->has_operand = 0;
            }
            if (strcmp(input, "=") == 0) {
                calc->state = DONE;
                return 1;
            } else if (strlen(input) == 1 && is_valid_operator(input[0])) {
                calc->operator = input[0];
                calc->state = READING_OPERATOR;
                return 1;
            }
            calc->state = ERROR;
            return 0;
            
        case ERROR:
        case DONE:
            return 0;
    }
    return 0;
}

int main(void) {
    struct Calculator calc;
    char input[256];
    
    init_calculator(&calc);
    
    printf("Simple Calculator (enter numbers and operators, = to finish)\n");
    printf("Current value: %.2f\n", calc.current_value);
    
    while (calc.state != DONE && calc.state != ERROR) {
        printf("Enter input: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (!process_input(&calc, input)) {
            if (calc.state == ERROR) {
                printf("Error: Invalid input sequence\n");
                break;
            }
        }
        
        if (calc.state == CALCULATING || calc.state == READING_NUMBER) {
            printf("Current value: %.2f\n", calc.current_value);
        }
    }
    
    if (calc.state == DONE) {
        if (calc.has_operand) {
            calc.current_value = perform_operation(calc.current_value, calc.operand, calc.operator);
        }
        printf("Final result: %.2f\n", calc.current_value);
    }
    
    return 0;
}