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
    char *endptr;
    double number;
    
    switch (calc->state) {
        case INIT:
        case READING_NUMBER:
            number = strtod(input, &endptr);
            if (endptr != input && *endptr == '\0') {
                if (calc->state == INIT) {
                    calc->current_value = number;
                } else {
                    if (calc->has_operand && is_valid_operator(calc->operator)) {
                        calc->current_value = perform_operation(
                            calc->current_value, number, calc->operator);
                        if (calc->operator == '/' && number == 0.0) {
                            calc->state = ERROR;
                            return 0;
                        }
                    }
                    calc->has_operand = 0;
                }
                calc->state = READING_OPERATOR;
                return 1;
            }
            calc->state = ERROR;
            return 0;
            
        case READING_OPERATOR:
            if (strlen(input) == 1 && is_valid_operator(input[0])) {
                calc->operator = input[0];
                calc->state = READING_NUMBER;
                calc->has_operand = 1;
                return 1;
            } else if (strcmp(input, "=") == 0) {
                calc->state = DONE;
                return 1;
            }
            calc->state = ERROR;
            return 0;
            
        case CALCULATING:
        case ERROR:
        case DONE:
        default:
            return 0;
    }
}

int main(void) {
    struct Calculator calc;
    char input[256];
    
    init_calculator(&calc);
    
    printf("Simple Calculator State Machine\n");
    printf("Enter numbers and operators (+, -, *, /), end with '='\n");
    printf("Current value: %.2f\n", calc.current_value);
    
    while (calc.state != DONE && calc.state != ERROR) {
        printf("> ");
        
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
                printf("Error: Invalid input or operation\n");
                break;
            }
        }
        
        if (calc.state == READING_OPERATOR || calc.state == READING_NUMBER) {
            printf("Current value: %.2f\n", calc.current_value);
        }
    }
    
    if (calc.state == DONE) {
        printf("Final result: %.2f\n", calc.current_value);
    } else if (calc.state != ERROR) {
        printf("Calculation incomplete\n");
    }
    
    return 0;
}