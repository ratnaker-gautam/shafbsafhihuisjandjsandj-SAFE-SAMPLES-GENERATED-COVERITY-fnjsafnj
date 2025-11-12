//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUM,
    READING_OP,
    CALCULATING,
    FINISHED,
    ERROR
};

struct Calculator {
    enum State state;
    int64_t current_value;
    int64_t accumulator;
    char last_operator;
    int error_code;
};

void init_calculator(struct Calculator *calc) {
    calc->state = INIT;
    calc->current_value = 0;
    calc->accumulator = 0;
    calc->last_operator = '+';
    calc->error_code = 0;
}

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int64_t safe_add(int64_t a, int64_t b) {
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) {
        return 0;
    }
    return a + b;
}

int64_t safe_multiply(int64_t a, int64_t b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT64_MAX / b) return 0;
        } else {
            if (b < INT64_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT64_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT64_MAX / a) return 0;
        }
    }
    return a * b;
}

void process_calculation(struct Calculator *calc) {
    switch (calc->last_operator) {
        case '+':
            calc->accumulator = safe_add(calc->accumulator, calc->current_value);
            if (calc->accumulator == 0 && calc->current_value != 0) {
                calc->state = ERROR;
                calc->error_code = 1;
            }
            break;
        case '-':
            calc->accumulator = safe_add(calc->accumulator, -calc->current_value);
            if (calc->accumulator == 0 && calc->current_value != 0) {
                calc->state = ERROR;
                calc->error_code = 1;
            }
            break;
        case '*':
            calc->accumulator = safe_multiply(calc->accumulator, calc->current_value);
            if (calc->accumulator == 0 && calc->accumulator != 0 && calc->current_value != 0) {
                calc->state = ERROR;
                calc->error_code = 2;
            }
            break;
        case '/':
            if (calc->current_value == 0) {
                calc->state = ERROR;
                calc->error_code = 3;
            } else {
                calc->accumulator /= calc->current_value;
            }
            break;
    }
}

int main(void) {
    struct Calculator calc;
    init_calculator(&calc);
    char input[256];
    int pos = 0;
    int ch;
    
    printf("Enter calculation (e.g., 5+3*2): ");
    
    while ((ch = getchar()) != '\n' && ch != EOF && pos < 255) {
        if (isspace(ch)) continue;
        
        input[pos++] = (char)ch;
    }
    input[pos] = '\0';
    
    if (pos == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    pos = 0;
    
    while (input[pos] != '\0' && calc.state != ERROR && calc.state != FINISHED) {
        switch (calc.state) {
            case INIT:
                if (isdigit(input[pos])) {
                    calc.current_value = 0;
                    while (isdigit(input[pos])) {
                        calc.current_value = calc.current_value * 10 + (input[pos] - '0');
                        pos++;
                    }
                    calc.accumulator = calc.current_value;
                    calc.state = READING_OP;
                } else {
                    calc.state = ERROR;
                    calc.error_code = 4;
                }
                break;
                
            case READING_OP:
                if (is_valid_operator(input[pos])) {
                    calc.last_operator = input[pos];
                    pos++;
                    calc.state = READING_NUM;
                } else if (input[pos] == '\0') {
                    calc.state = FINISHED;
                } else {
                    calc.state = ERROR;
                    calc.error_code = 5;
                }
                break;
                
            case READING_NUM:
                if (isdigit(input[pos])) {
                    calc.current_value = 0;
                    while (isdigit(input[pos])) {
                        calc.current_value = calc.current_value * 10 + (input[pos] - '0');
                        pos++;
                    }
                    calc.state = CALCULATING;
                } else {
                    calc.state = ERROR;
                    calc.error_code = 6;
                }
                break;
                
            case CALCULATING:
                process_calculation(&calc);
                if (calc.state != ERROR) {
                    calc.state = READING_OP;
                }
                break;
                
            default:
                break;
        }
    }
    
    if (calc.state == ERROR) {
        printf("Error: ");
        switch (calc.error_code) {
            case 1: printf("Integer overflow in addition/subtraction\n"); break;
            case 2: printf("Integer overflow in multiplication\n"); break;
            case 3: printf("