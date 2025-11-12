//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READING_NUM,
    READING_OP,
    CALCULATING,
    DONE,
    ERROR
};

struct calculator {
    enum state current;
    int32_t accumulator;
    int32_t operand;
    char operator;
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
    calc->current = INIT;
    calc->accumulator = 0;
    calc->operand = 0;
    calc->operator = '+';
    calc->error_code = 0;
}

void process_state(struct calculator *calc, char input) {
    switch (calc->current) {
        case INIT:
            if (isdigit(input)) {
                calc->accumulator = input - '0';
                calc->current = READING_NUM;
            } else if (input == 'q') {
                calc->current = DONE;
            } else {
                calc->current = ERROR;
                calc->error_code = 1;
            }
            break;
            
        case READING_NUM:
            if (isdigit(input)) {
                int32_t digit = input - '0';
                calc->accumulator = safe_mul(calc->accumulator, 10);
                calc->accumulator = safe_add(calc->accumulator, digit);
                if (calc->accumulator == 0) {
                    calc->current = ERROR;
                    calc->error_code = 2;
                }
            } else if (input == '+' || input == '-' || input == '*') {
                calc->operator = input;
                calc->current = READING_OP;
            } else if (input == '=') {
                calc->current = DONE;
            } else if (input == 'q') {
                calc->current = DONE;
            } else {
                calc->current = ERROR;
                calc->error_code = 3;
            }
            break;
            
        case READING_OP:
            if (isdigit(input)) {
                calc->operand = input - '0';
                calc->current = CALCULATING;
            } else if (input == 'q') {
                calc->current = DONE;
            } else {
                calc->current = ERROR;
                calc->error_code = 4;
            }
            break;
            
        case CALCULATING:
            if (isdigit(input)) {
                int32_t digit = input - '0';
                calc->operand = safe_mul(calc->operand, 10);
                calc->operand = safe_add(calc->operand, digit);
                if (calc->operand == 0) {
                    calc->current = ERROR;
                    calc->error_code = 5;
                }
            } else if (input == '+' || input == '-' || input == '*') {
                switch (calc->operator) {
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
                if (calc->accumulator == 0) {
                    calc->current = ERROR;
                    calc->error_code = 6;
                } else {
                    calc->operator = input;
                    calc->operand = 0;
                    calc->current = READING_OP;
                }
            } else if (input == '=') {
                switch (calc->operator) {
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
                if (calc->accumulator == 0) {
                    calc->current = ERROR;
                    calc->error_code = 7;
                } else {
                    calc->current = DONE;
                }
            } else if (input == 'q') {
                calc->current = DONE;
            } else {
                calc->current = ERROR;
                calc->error_code =