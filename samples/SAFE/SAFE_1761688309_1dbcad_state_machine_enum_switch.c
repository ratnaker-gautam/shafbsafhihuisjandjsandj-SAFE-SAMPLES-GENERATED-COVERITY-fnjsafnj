//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_OPERATOR,
    CALCULATING,
    ERROR,
    DONE
};

struct calculator {
    enum state current_state;
    double accumulator;
    double current_number;
    char current_operator;
    int has_operand;
};

void init_calculator(struct calculator *calc) {
    calc->current_state = INIT;
    calc->accumulator = 0.0;
    calc->current_number = 0.0;
    calc->current_operator = '+';
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
            if (b == 0.0) return 0.0;
            return a / b;
        default: return 0.0;
    }
}

int process_input(struct calculator *calc, const char *input) {
    char *endptr;
    double number;
    
    if (strlen(input) == 0) return 0;
    
    switch (calc->current_state) {
        case INIT:
        case READING_OPERATOR:
            number = strtod(input, &endptr);
            if (endptr != input) {
                calc->current_number = number;
                calc->has_operand = 1;
                calc->current_state = READING_NUMBER;
                return 1;
            }
            break;
            
        case READING_NUMBER:
            if (strlen(input) == 1 && is_valid_operator(input[0])) {
                calc->current_operator = input[0];
                calc->current_state = READING_OPERATOR;
                return 1;
            }
            break;
            
        case CALCULATING:
            calc->current_state = DONE;
            return 1;
            
        default:
            break;
    }
    return 0;
}

void calculate(struct calculator *calc) {
    if (calc->current_state == READING_NUMBER && calc->has_operand) {
        calc->accumulator = perform_operation(calc->accumulator, calc->current_number, calc->current_operator);
        calc->has_operand = 0;
        calc->current_state = CALCULATING;
    }
}

int main(void) {
    struct calculator calc;
    char input[256];
    int running = 1;
    
    init_calculator(&calc);
    printf("Simple Calculator (enter '=' to calculate, 'q' to quit)\n");
    printf("Start with a number: ");
    
    while (running) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "q") == 0) {
            break;
        }
        
        if (strcmp(input, "=") == 0) {
            calculate(&calc);
            if (calc.current_state == CALCULATING) {
                printf("Result: %.2f\n", calc.accumulator);
                calc.current_state = DONE;
            }
            printf("Enter 'q' to quit or new calculation: ");
            init_calculator(&calc);
            continue;
        }
        
        if (!process_input(&calc, input)) {
            printf("Invalid input. Please enter a number or operator (+, -, *, /)\n");
            calc.current_state = ERROR;
        } else {
            calculate(&calc);
        }
        
        switch (calc.current_state) {
            case READING_NUMBER:
                printf("Enter operator: ");
                break;
            case READING_OPERATOR:
                printf("Enter number: ");
                break;
            case ERROR:
                printf("Try again: ");
                calc.current_state = INIT;
                break;
            default:
                break;
        }
    }
    
    return 0;
}