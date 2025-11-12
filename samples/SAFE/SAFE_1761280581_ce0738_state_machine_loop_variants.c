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
    char operator;
    int valid;
};

void init_calculator(struct calculator *calc) {
    calc->current = INIT;
    calc->accumulator = 0;
    calc->operand = 0;
    calc->operator = '+';
    calc->valid = 1;
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

int process_input(struct calculator *calc, const char *input) {
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len) {
        switch (calc->current) {
            case INIT:
                if (isdigit(input[i])) {
                    calc->current = READING_NUM;
                    calc->accumulator = 0;
                    i--;
                } else if (isspace(input[i])) {
                    i++;
                } else {
                    calc->current = ERROR;
                }
                break;
                
            case READING_NUM:
                {
                    int32_t num = 0;
                    int sign = 1;
                    int digits = 0;
                    
                    if (i < len && input[i] == '-') {
                        sign = -1;
                        i++;
                    }
                    
                    while (i < len && isdigit(input[i])) {
                        int digit = input[i] - '0';
                        if (num > (INT32_MAX - digit) / 10) {
                            calc->current = ERROR;
                            break;
                        }
                        num = num * 10 + digit;
                        digits++;
                        i++;
                    }
                    
                    if (digits > 0 && calc->current != ERROR) {
                        num *= sign;
                        if (calc->operator == '+') {
                            calc->accumulator = safe_add(calc->accumulator, num);
                        } else if (calc->operator == '-') {
                            calc->accumulator = safe_sub(calc->accumulator, num);
                        } else if (calc->operator == '*') {
                            calc->accumulator = safe_mul(calc->accumulator, num);
                        }
                        
                        if (calc->accumulator == 0 && calc->valid) {
                            calc->current = ERROR;
                        } else {
                            calc->current = READING_OP;
                        }
                    } else {
                        calc->current = ERROR;
                    }
                }
                break;
                
            case READING_OP:
                if (i < len && (input[i] == '+' || input[i] == '-' || input[i] == '*')) {
                    calc->operator = input[i];
                    calc->current = READING_NUM;
                    i++;
                } else if (i < len && input[i] == '=') {
                    calc->current = DONE;
                    i++;
                } else if (isspace(input[i])) {
                    i++;
                } else {
                    calc->current = ERROR;
                }
                break;
                
            case CALCULATING:
                calc->current = READING_OP;
                break;
                
            case DONE:
                i = len;
                break;
                
            case ERROR:
                i = len;
                break;
        }
        
        if (calc->current == ERROR) {
            calc->valid = 0;
            break;
        }
    }
    
    return calc->valid;
}

int main(void) {
    struct calculator calc;
    char input[256];
    
    printf("Enter calculation (e.g., 5 + 3 * 2 = ): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    init_calculator(&calc);
    
    int result = process_input(&calc, input);
    
    if (result && calc.current == DONE) {
        printf("Result: %d\n", calc.accumulator);
    } else {
        printf("Error: Invalid calculation\n");
        return 1;
    }
    
    return 0;
}