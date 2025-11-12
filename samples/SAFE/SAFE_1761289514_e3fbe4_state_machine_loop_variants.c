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
    char operator = '+';
    char input_buffer[256];
    size_t input_len = 0;
    int i = 0;
    
    printf("Enter arithmetic expression (numbers and +- operators): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    while (i < input_len) {
        char c = input_buffer[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_value = c - '0';
                    current_state = READING_NUM;
                } else if (c == ' ') {
                    i++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (current_value > (INT_MAX - (c - '0')) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    current_value = current_value * 10 + (c - '0');
                } else if (c == '+' || c == '-') {
                    if (operator == '+') {
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
                    operator = c;
                    current_value = 0;
                    current_state = READING_OP;
                } else if (c == ' ') {
                    current_state = READING_OP;
                } else if (c == '\0') {
                    if (operator == '+') {
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
                    i++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                i++;
                continue;
                
            case ERROR:
                fprintf(stderr, "Invalid expression format\n");
                return 1;
        }
        
        i++;
        
        if (current_state == FINISHED) {
            break;
        }
    }
    
    if (current_state == READING_NUM) {
        if (operator == '+') {
            if (accumulator > INT_MAX - current_value) {
                fprintf(stderr, "Integer overflow\n");
                return 1;
            }
            accumulator += current_value;
        } else {
            if (accumulator < INT_MIN + current_value) {
                fprintf(stderr, "Integer underflow\n");
                return 1;
            }
            accumulator -= current_value;
        }
        current_state = FINISHED;
    }
    
    if (current_state != FINISHED && current_state != ERROR) {
        fprintf(stderr, "Unexpected end of expression\n");
        return 1;
    }
    
    if (current_state == FINISHED) {
        printf("Result: %d\n", accumulator);
    }
    
    return 0;
}