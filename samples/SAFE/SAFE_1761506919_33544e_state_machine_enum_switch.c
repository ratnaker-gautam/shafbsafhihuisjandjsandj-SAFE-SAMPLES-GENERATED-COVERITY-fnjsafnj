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
    double operand1;
    double operand2;
    char operator;
    double result;
    int error_code;
};

void init_calculator(struct calculator *calc) {
    calc->current_state = INIT;
    calc->operand1 = 0.0;
    calc->operand2 = 0.0;
    calc->operator = '\0';
    calc->result = 0.0;
    calc->error_code = 0;
}

int is_valid_operator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}

int parse_number(const char *input, double *number) {
    char *endptr;
    *number = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    return 1;
}

void process_calculation(struct calculator *calc) {
    switch (calc->operator) {
        case '+':
            calc->result = calc->operand1 + calc->operand2;
            break;
        case '-':
            calc->result = calc->operand1 - calc->operand2;
            break;
        case '*':
            calc->result = calc->operand1 * calc->operand2;
            break;
        case '/':
            if (calc->operand2 == 0.0) {
                calc->error_code = 1;
                calc->current_state = ERROR;
                return;
            }
            calc->result = calc->operand1 / calc->operand2;
            break;
        default:
            calc->error_code = 2;
            calc->current_state = ERROR;
            return;
    }
    calc->current_state = DONE;
}

void run_state_machine(struct calculator *calc) {
    char input_buffer[256];
    
    while (calc->current_state != DONE && calc->current_state != ERROR) {
        switch (calc->current_state) {
            case INIT:
                printf("Enter first number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    calc->error_code = 3;
                    calc->current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!parse_number(input_buffer, &calc->operand1)) {
                    calc->error_code = 4;
                    calc->current_state = ERROR;
                    break;
                }
                calc->current_state = READING_OPERATOR;
                break;
                
            case READING_OPERATOR:
                printf("Enter operator (+, -, *, /): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    calc->error_code = 3;
                    calc->current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strlen(input_buffer) != 1 || !is_valid_operator(input_buffer[0])) {
                    calc->error_code = 5;
                    calc->current_state = ERROR;
                    break;
                }
                calc->operator = input_buffer[0];
                calc->current_state = READING_NUMBER;
                break;
                
            case READING_NUMBER:
                printf("Enter second number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    calc->error_code = 3;
                    calc->current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!parse_number(input_buffer, &calc->operand2)) {
                    calc->error_code = 4;
                    calc->current_state = ERROR;
                    break;
                }
                calc->current_state = CALCULATING;
                break;
                
            case CALCULATING:
                process_calculation(calc);
                break;
                
            default:
                calc->error_code = 6;
                calc->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    struct calculator calc;
    init_calculator(&calc);
    
    run_state_machine(&calc);
    
    if (calc.current_state == DONE) {
        printf("Result: %.2f\n", calc.result);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Calculation failed (code %d)\n", calc.error_code);
        return EXIT_FAILURE;
    }
}