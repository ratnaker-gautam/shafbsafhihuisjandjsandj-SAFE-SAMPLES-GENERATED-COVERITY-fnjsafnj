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
    int valid;
};

int main(void) {
    struct calculator calc = {INIT, 0, 0, '+', 1};
    char input[256];
    char *ptr;
    size_t len;
    int i;
    
    printf("Enter calculation (e.g., 5 + 3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    ptr = input;
    i = 0;
    
    while (calc.current != DONE && calc.current != ERROR && i < (int)len) {
        switch (calc.current) {
            case INIT:
                while (i < (int)len && isspace(ptr[i])) i++;
                if (i >= (int)len) {
                    calc.current = ERROR;
                    break;
                }
                calc.current = READING_NUM;
                break;
                
            case READING_NUM:
                {
                    char *end;
                    long value;
                    
                    while (i < (int)len && isspace(ptr[i])) i++;
                    if (i >= (int)len) {
                        calc.current = ERROR;
                        break;
                    }
                    
                    value = strtol(&ptr[i], &end, 10);
                    if (end == &ptr[i]) {
                        calc.current = ERROR;
                        break;
                    }
                    
                    if (value < INT32_MIN || value > INT32_MAX) {
                        calc.current = ERROR;
                        break;
                    }
                    
                    calc.operand = (int32_t)value;
                    i += (int)(end - &ptr[i]);
                    
                    if (calc.operation == '+') {
                        if (calc.accumulator > 0 && calc.operand > INT32_MAX - calc.accumulator) {
                            calc.current = ERROR;
                            break;
                        }
                        if (calc.accumulator < 0 && calc.operand < INT32_MIN - calc.accumulator) {
                            calc.current = ERROR;
                            break;
                        }
                        calc.accumulator += calc.operand;
                    } else if (calc.operation == '-') {
                        if (calc.accumulator > 0 && calc.operand < calc.accumulator - INT32_MAX) {
                            calc.current = ERROR;
                            break;
                        }
                        if (calc.accumulator < 0 && calc.operand > calc.accumulator - INT32_MIN) {
                            calc.current = ERROR;
                            break;
                        }
                        calc.accumulator -= calc.operand;
                    } else {
                        calc.current = ERROR;
                        break;
                    }
                    
                    calc.current = READING_OP;
                }
                break;
                
            case READING_OP:
                while (i < (int)len && isspace(ptr[i])) i++;
                if (i >= (int)len) {
                    calc.current = DONE;
                    break;
                }
                
                if (ptr[i] == '+' || ptr[i] == '-') {
                    calc.operation = ptr[i];
                    i++;
                    calc.current = READING_NUM;
                } else if (ptr[i] == '=') {
                    i++;
                    calc.current = DONE;
                } else {
                    calc.current = ERROR;
                }
                break;
                
            case CALCULATING:
            case DONE:
            case ERROR:
                break;
        }
    }
    
    if (calc.current == ERROR) {
        printf("Error: Invalid calculation\n");
        return 1;
    }
    
    if (calc.current != DONE) {
        printf("Error: Incomplete calculation\n");
        return 1;
    }
    
    printf("Result: %d\n", calc.accumulator);
    return 0;
}