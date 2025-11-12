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
    enum state current_state;
    int64_t accumulator;
    int64_t operand;
    char operation;
    int error_code;
};

void init_calculator(struct calculator *calc) {
    calc->current_state = INIT;
    calc->accumulator = 0;
    calc->operand = 0;
    calc->operation = '+';
    calc->error_code = 0;
}

int is_valid_operator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}

int64_t safe_add(int64_t a, int64_t b) {
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) {
        return 0;
    }
    return a + b;
}

int64_t safe_sub(int64_t a, int64_t b) {
    if ((b > 0 && a < INT64_MIN + b) || (b < 0 && a > INT64_MAX + b)) {
        return 0;
    }
    return a - b;
}

int64_t safe_mul(int64_t a, int64_t b) {
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

int64_t safe_div(int64_t a, int64_t b) {
    if (b == 0) return 0;
    if (a == INT64_MIN && b == -1) return 0;
    return a / b;
}

void perform_calculation(struct calculator *calc) {
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
        case '/':
            if (calc->operand == 0) {
                calc->error_code = 1;
                calc->current_state = ERROR;
                return;
            }
            calc->accumulator = safe_div(calc->accumulator, calc->operand);
            break;
    }
    calc->current_state = READING_OP;
}

int process_input(struct calculator *calc, const char *input) {
    size_t len = strlen(input);
    if (len == 0 || len > 32) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (calc->current_state) {
            case INIT:
            case READING_OP:
                if (isdigit(c) || (c == '-' && i == 0)) {
                    calc->current_state = READING_NUM;
                    char *endptr;
                    calc->operand = strtoll(input + i, &endptr, 10);
                    if (endptr == input + i || calc->operand == LLONG_MAX || calc->operand == LLONG_MIN) {
                        calc->current_state = ERROR;
                        calc->error_code = 2;
                        return 0;
                    }
                    i = endptr - input - 1;
                    if (calc->current_state == READING_OP) {
                        calc->current_state = CALCULATING;
                    } else {
                        calc->accumulator = calc->operand;
                        calc->current_state = READING_OP;
                    }
                } else if (is_valid_operator(c) && calc->current_state == READING_OP) {
                    calc->operation = c;
                } else if (c == '=') {
                    calc->current_state = DONE;
                    return 1;
                } else if (!isspace(c)) {
                    calc->current_state = ERROR;
                    calc->error_code = 3;
                    return 0;
                }
                break;
                
            case READING_NUM:
                if (!isdigit(c) && !isspace(c) && c != '=' && !is_valid_operator(c)) {
                    calc->current_state = ERROR;
                    calc->error_code = 4;
                    return 0;
                }
                i--;
                calc->current_state = READING_OP;
                break;
                
            case CALCULATING:
                perform_calculation(calc);
                if (calc->current_state == ERROR) return 0;
                i--;
                break;
                
            case DONE:
            case ERROR:
                return calc->current_state == DONE;
        }
    }
    
    if (calc->current_state == CALCULATING) {
        perform_calculation(calc);
    }
    
    return calc->current_state == DONE;
}

int main(void) {
    struct calculator calc;
    char input[256];
    int result_available = 0;
    
    init