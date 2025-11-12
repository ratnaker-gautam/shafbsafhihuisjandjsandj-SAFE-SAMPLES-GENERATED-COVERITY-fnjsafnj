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
    enum state current;
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

void process_calculation(struct calculator *calc) {
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
        default:
            calc->error_code = 3;
            calc->current = ERROR;
            return;
    }
    if (calc->accumulator == 0 && calc->error_code == 0) {
        calc->error_code = 4;
    }
}

int main(void) {
    struct calculator calc = {INIT, 0, 0, '+', 0};
    char input[256];
    char *ptr;
    int valid_input;
    
    printf("Simple Calculator (+, -, *)\n");
    printf("Enter expressions like '5 + 3 * 2' or 'q' to quit\n");
    
    while (calc.current != DONE && calc.current != ERROR) {
        valid_input = 0;
        
        switch (calc.current) {
            case INIT:
                printf("Enter first number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    calc.current = DONE;
                    break;
                }
                if (input[0] == 'q' || input[0] == 'Q') {
                    calc.current = DONE;
                    break;
                }
                calc.accumulator = strtol(input, &ptr, 10);
                if (ptr == input || *ptr != '\n') {
                    calc.error_code = 1;
                    calc.current = ERROR;
                } else {
                    calc.current = READING_OP;
                }
                break;
                
            case READING_OP:
                printf("Enter operation (+, -, *, =): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    calc.current = DONE;
                    break;
                }
                if (input[0] == '=') {
                    calc.current = DONE;
                    break;
                }
                if (input[0] == '+' || input[0] == '-' || input[0] == '*') {
                    calc.operation = input[0];
                    calc.current = READING_NUM;
                } else {
                    calc.error_code = 2;
                    calc.current = ERROR;
                }
                break;
                
            case READING_NUM:
                printf("Enter next number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    calc.current = DONE;
                    break;
                }
                calc.operand = strtol(input, &ptr, 10);
                if (ptr == input || *ptr != '\n') {
                    calc.error_code = 1;
                    calc.current = ERROR;
                } else {
                    calc.current = CALCULATING;
                }
                break;
                
            case CALCULATING:
                process_calculation(&calc);
                if (calc.current != ERROR) {
                    printf("Current result: %d\n", calc.accumulator);
                    calc.current = READING_OP;
                }
                break;
                
            default:
                calc.current = ERROR;
                break;
        }
    }
    
    if (calc.current == ERROR) {
        printf("Error %d occurred\n", calc.error_code);
        return 1;
    }
    
    printf("Final result: %d\n", calc.accumulator);
    return 0;
}