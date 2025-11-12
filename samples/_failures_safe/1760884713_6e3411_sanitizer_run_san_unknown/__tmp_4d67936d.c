//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUM,
    READING_OP,
    ERROR,
    DONE
};

struct Calculator {
    enum State state;
    double current_value;
    double operand;
    char operation;
    int has_operand;
};

void init_calculator(struct Calculator *calc) {
    calc->state = INIT;
    calc->current_value = 0.0;
    calc->operand = 0.0;
    calc->operation = '\0';
    calc->has_operand = 0;
}

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int process_input(struct Calculator *calc, const char *input) {
    size_t len = strlen(input);
    if (len == 0) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (calc->state) {
            case INIT:
                if (isdigit(c) || c == '.' || c == '-') {
                    calc->state = READING_NUM;
                    i--;
                } else if (isspace(c)) {
                    continue;
                } else {
                    calc->state = ERROR;
                    return 0;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c) || c == '.' || (c == '-' && i == 0)) {
                    continue;
                } else if (isspace(c)) {
                    char *endptr;
                    double num = strtod(input, &endptr);
                    if (endptr == input) {
                        calc->state = ERROR;
                        return 0;
                    }
                    
                    if (!calc->has_operand) {
                        calc->current_value = num;
                        calc->has_operand = 1;
                    } else {
                        switch (calc->operation) {
                            case '+': calc->current_value += num; break;
                            case '-': calc->current_value -= num; break;
                            case '*': calc->current_value *= num; break;
                            case '/': 
                                if (num == 0.0) {
                                    calc->state = ERROR;
                                    return 0;
                                }
                                calc->current_value /= num; 
                                break;
                        }
                    }
                    calc->state = READING_OP;
                } else if (is_valid_operator(c)) {
                    calc->operation = c;
                    calc->state = INIT;
                    calc->has_operand = 1;
                } else {
                    calc->state = ERROR;
                    return 0;
                }
                break;
                
            case READING_OP:
                if (is_valid_operator(c)) {
                    calc->operation = c;
                    calc->state = INIT;
                } else if (isspace(c)) {
                    continue;
                } else {
                    calc->state = ERROR;
                    return 0;
                }
                break;
                
            case ERROR:
                return 0;
                
            case DONE:
                return 1;
        }
    }
    
    return 1;
}

int main(void) {
    struct Calculator calc;
    init_calculator(&calc);
    
    char input[256];
    printf("Simple Calculator (enter '=' to finish, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        
        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strcmp(input, "=") == 0) {
            if (calc.state != ERROR && calc.has_operand) {
                printf("Result: %.2f\n", calc.current_value);
                calc.state = DONE;
            } else {
                printf("Error: Invalid expression\n");
            }
            init_calculator(&calc);
            printf("Enter new expression: ");
            continue;
        }
        
        if (!process_input(&calc, input)) {
            printf("Error: Invalid input\n");
            init_calculator(&calc);
        }
        
        printf("Enter next part or '=' for result: ");
    }
    
    return 0;
}