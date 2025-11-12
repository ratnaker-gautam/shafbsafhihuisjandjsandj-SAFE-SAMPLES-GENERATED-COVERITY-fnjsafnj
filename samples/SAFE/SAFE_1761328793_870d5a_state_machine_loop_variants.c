//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUM, READING_OP, FINISHED, ERROR };

int main(void) {
    enum State current_state = START;
    int accumulator = 0;
    int current_value = 0;
    char op = '+';
    char input[256];
    int pos = 0;
    
    printf("Enter simple arithmetic expression (numbers and +- operators): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    while (pos < len && current_state != ERROR && current_state != FINISHED) {
        char c = input[pos];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_value = c - '0';
                    current_state = READING_NUM;
                } else if (c == ' ') {
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    int digit = c - '0';
                    if (current_value > (INT_MAX - digit) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    current_value = current_value * 10 + digit;
                } else if (c == '+' || c == '-') {
                    if (op == '+') {
                        if (accumulator > INT_MAX - current_value) {
                            current_state = ERROR;
                            break;
                        }
                        accumulator += current_value;
                    } else {
                        if (accumulator < INT_MIN + current_value) {
                            current_state = ERROR;
                            break;
                        }
                        accumulator -= current_value;
                    }
                    op = c;
                    current_value = 0;
                    current_state = READING_OP;
                } else if (c == ' ') {
                    current_state = READING_OP;
                } else if (c == '\0') {
                    if (op == '+') {
                        if (accumulator > INT_MAX - current_value) {
                            current_state = ERROR;
                            break;
                        }
                        accumulator += current_value;
                    } else {
                        if (accumulator < INT_MIN + current_value) {
                            current_state = ERROR;
                            break;
                        }
                        accumulator -= current_value;
                    }
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    current_value = c - '0';
                    current_state = READING_NUM;
                } else if (c == ' ') {
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
        
        pos++;
    }
    
    if (current_state == READING_NUM) {
        if (op == '+') {
            if (accumulator > INT_MAX - current_value) {
                current_state = ERROR;
            } else {
                accumulator += current_value;
                current_state = FINISHED;
            }
        } else {
            if (accumulator < INT_MIN + current_value) {
                current_state = ERROR;
            } else {
                accumulator -= current_value;
                current_state = FINISHED;
            }
        }
    }
    
    if (current_state == FINISHED) {
        printf("Result: %d\n", accumulator);
    } else {
        printf("Error: Invalid expression\n");
        return 1;
    }
    
    return 0;
}