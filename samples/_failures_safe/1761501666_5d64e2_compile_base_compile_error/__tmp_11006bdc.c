//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUM,
    READING_OP,
    CALCULATING,
    DONE,
    ERROR
};

struct calculator {
    enum state current_state;
    int32_t accumulator;
    int32_t operand;
    char operation;
    int error_code;
};

int32_t safe_add(int32_t a, int32_t b) {
    if ((b > 0 && a > INT32_MAX - b) || (b < 0 && a < INT32_MIN - b)) {
        return 0;
    }
    return a + b;
}

int32_t safe_sub(int32_t a, int32_t b) {
    if ((b > 0 && a < INT32_MIN + b) || (b < 0 && a > INT32_MAX + b)) {
        return 0;
    }
    return a - b;
}

int32_t safe_mul(int32_t a, int32_t b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT32_MAX / b) return 0;
        } else {
            if (b < INT32_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT32_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT32_MAX / a) return 0;
        }
    }
    return a * b;
}

void state_machine_init(struct calculator *calc) {
    calc->current_state = INIT;
    calc->accumulator = 0;
    calc->operand = 0;
    calc->operation = '+';
    calc->error_code = 0;
}

void process_state(struct calculator *calc, char input) {
    switch (calc->current_state) {
        case INIT:
            if (isdigit(input)) {
                calc->operand = input - '0';
                calc->current_state = READING_NUM;
            } else if (input == 'q') {
                calc->current_state = DONE;
            } else {
                calc->current_state = ERROR;
                calc->error_code = 1;
            }
            break;
            
        case READING_NUM:
            if (isdigit(input)) {
                int32_t digit = input - '0';
                calc->operand = safe_mul(calc->operand, 10);
                calc->operand = safe_add(calc->operand, digit);
                if (calc->operand == 0 && digit != 0) {
                    calc->current_state = ERROR;
                    calc->error_code = 2;
                }
            } else if (input == '+' || input == '-' || input == '*') {
                calc->operation = input;
                calc->current_state = CALCULATING;
            } else if (input == '=') {
                calc->current_state = CALCULATING;
            } else if (input == 'q') {
                calc->current_state = DONE;
            } else {
                calc->current_state = ERROR;
                calc->error_code = 3;
            }
            break;
            
        case CALCULATING:
            switch (calc->operation) {
                case '+':
                    calc->accumulator = safe_add(calc->accumulator, calc->operand);
                    break;
                case '-':
                    calc->accumulator = safe_sub(calc->accumulator, calc->operand);
                    break;
                case '*':
                    calc->accumulator = safe_mul(calc->accumulator, calc->operand);
                    break;
            }
            if (calc->accumulator == 0 && calc->operand != 0) {
                calc->current_state = ERROR;
                calc->error_code = 4;
                break;
            }
            calc->operand = 0;
            if (input == '=') {
                calc->current_state = DONE;
            } else if (isdigit(input)) {
                calc->operand = input - '0';
                calc->current_state = READING_NUM;
            } else if (input == '+' || input == '-' || input == '*') {
                calc->operation = input;
                calc->current_state = READING_OP;
            } else if (input == 'q') {
                calc->current_state = DONE;
            } else {
                calc->current_state = ERROR;
                calc->error_code = 5;
            }
            break;
            
        case READING_OP:
            if (input == '+' || input == '-' || input == '*') {
                calc->operation = input;
                calc->current_state = READING_NUM;
            } else if (isdigit(input)) {
                calc->operand = input - '0';
                calc->current_state = READING_NUM;
            } else if (input == 'q') {
                calc->current_state = DONE;
            } else {
                calc->current_state = ERROR;
                calc->error_code = 6;
            }
            break;
            
        case ERROR:
        case DONE:
            break;
    }
}

int main(void) {
    struct calculator calc;
    state_machine_init(&calc);
    
    printf("Calculator State Machine\n");
    printf("Enter expressions like: 5+3*2=\n");
    printf("Press q to quit\n");
    printf("Current result: 0\n");
    
    char input;
    int valid_input;
    
    while (calc.current_state != DONE && calc.current_state != ERROR) {