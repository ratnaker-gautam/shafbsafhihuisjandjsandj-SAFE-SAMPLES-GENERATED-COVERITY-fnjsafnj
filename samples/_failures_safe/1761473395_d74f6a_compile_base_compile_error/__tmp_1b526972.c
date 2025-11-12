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
    FINISHED,
    ERROR
};

struct calculator {
    enum state current_state;
    int64_t accumulator;
    int64_t operand;
    char operation;
    int valid;
};

void init_calculator(struct calculator *calc) {
    calc->current_state = INIT;
    calc->accumulator = 0;
    calc->operand = 0;
    calc->operation = '+';
    calc->valid = 1;
}

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int process_input(struct calculator *calc, const char *input) {
    size_t len = strlen(input);
    if (len == 0 || len > 100) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (calc->current_state) {
            case INIT:
                if (isdigit(c)) {
                    calc->accumulator = c - '0';
                    calc->current_state = READING_NUM;
                } else if (c == ' ') {
                    continue;
                } else {
                    calc->current_state = ERROR;
                    return 0;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (calc->accumulator > INT64_MAX / 10) {
                        calc->current_state = ERROR;
                        return 0;
                    }
                    calc->accumulator = calc->accumulator * 10 + (c - '0');
                } else if (is_valid_operator(c)) {
                    calc->operation = c;
                    calc->current_state = READING_OP;
                } else if (c == '=') {
                    calc->current_state = FINISHED;
                } else if (c == ' ') {
                    continue;
                } else {
                    calc->current_state = ERROR;
                    return 0;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    calc->operand = c - '0';
                    calc->current_state = CALCULATING;
                } else if (c == ' ') {
                    continue;
                } else {
                    calc->current_state = ERROR;
                    return 0;
                }
                break;
                
            case CALCULATING:
                if (isdigit(c)) {
                    if (calc->operand > INT64_MAX / 10) {
                        calc->current_state = ERROR;
                        return 0;
                    }
                    calc->operand = calc->operand * 10 + (c - '0');
                } else if (is_valid_operator(c)) {
                    switch (calc->operation) {
                        case '+':
                            if (calc->accumulator > INT64_MAX - calc->operand) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator += calc->operand;
                            break;
                        case '-':
                            if (calc->accumulator < INT64_MIN + calc->operand) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator -= calc->operand;
                            break;
                        case '*':
                            if (calc->operand != 0 && calc->accumulator > INT64_MAX / calc->operand) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator *= calc->operand;
                            break;
                        case '/':
                            if (calc->operand == 0) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator /= calc->operand;
                            break;
                    }
                    calc->operation = c;
                    calc->operand = 0;
                    calc->current_state = READING_OP;
                } else if (c == '=') {
                    switch (calc->operation) {
                        case '+':
                            if (calc->accumulator > INT64_MAX - calc->operand) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator += calc->operand;
                            break;
                        case '-':
                            if (calc->accumulator < INT64_MIN + calc->operand) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator -= calc->operand;
                            break;
                        case '*':
                            if (calc->operand != 0 && calc->accumulator > INT64_MAX / calc->operand) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator *= calc->operand;
                            break;
                        case '/':
                            if (calc->operand == 0) {
                                calc->current_state = ERROR;
                                return 0;
                            }
                            calc->accumulator /= calc->operand;
                            break;
                    }
                    calc->current_state = FINISHED;
                } else if (c == ' ') {
                    continue;
                } else {
                    calc->current_state = ERROR;
                    return 0;
                }
                break;
                
            case FINISHED:
                if (c != ' ') {
                    calc->current_state = ERROR;
                    return 0;
                }
                break;
                
            case ERROR:
                return 0;
        }
    }
    
    return calc->current_state == FINISHED;
}

int main() {
    struct calculator calc;