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
    int32_t accumulator;
    int32_t operand;
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

int32_t safe_div(int32_t a, int32_t b) {
    if (b == 0) return 0;
    if (a == INT32_MIN && b == -1) return 0;
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
            calc->accumulator = safe_div(calc->accumulator, calc->operand);
            break;
    }
    if (calc->accumulator == 0 && calc->operation != '+') {
        calc->current_state = ERROR;
        calc->error_code = 2;
    }
}

int main(void) {
    struct calculator calc;
    init_calculator(&calc);
    char input_buffer[256];
    int input_pos = 0;
    int reading_number = 0;
    int number_value = 0;
    int negative = 1;

    printf("Simple Calculator (enter '=' to finish):\n");
    
    while (calc.current_state != FINISHED && calc.current_state != ERROR) {
        int c = getchar();
        if (c == EOF) break;
        
        if (input_pos < 255) {
            input_buffer[input_pos++] = (char)c;
            input_buffer[input_pos] = '\0';
        }

        switch (calc.current_state) {
            case INIT:
                if (isdigit(c)) {
                    number_value = c - '0';
                    reading_number = 1;
                    calc.current_state = READING_NUM;
                } else if (c == '-') {
                    negative = -1;
                    reading_number = 1;
                    calc.current_state = READING_NUM;
                } else if (c == '=') {
                    calc.current_state = FINISHED;
                } else if (!isspace(c)) {
                    calc.current_state = ERROR;
                    calc.error_code = 1;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    int digit = c - '0';
                    if (number_value > INT32_MAX / 10) {
                        calc.current_state = ERROR;
                        calc.error_code = 3;
                        break;
                    }
                    number_value = number_value * 10 + digit;
                    if (number_value < 0) {
                        calc.current_state = ERROR;
                        calc.error_code = 3;
                    }
                } else {
                    calc.operand = number_value * negative;
                    negative = 1;
                    number_value = 0;
                    reading_number = 0;
                    
                    if (is_valid_operator((char)c)) {
                        calc.operation = (char)c;
                        calc.current_state = READING_OP;
                    } else if (c == '=') {
                        perform_calculation(&calc);
                        calc.current_state = FINISHED;
                    } else if (!isspace(c)) {
                        calc.current_state = ERROR;
                        calc.error_code = 1;
                    }
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    number_value = c - '0';
                    reading_number = 1;
                    calc.current_state = READING_NUM;
                } else if (c == '-') {
                    negative = -1;
                    reading