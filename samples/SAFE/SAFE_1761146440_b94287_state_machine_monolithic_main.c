//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUM,
    READING_OP,
    CALCULATING,
    ERROR,
    DONE
};

int main(void) {
    enum State state = INIT;
    double result = 0.0;
    double current_num = 0.0;
    char current_op = '+';
    char input[256];
    int pos = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    int sign = 1;
    
    printf("Simple Calculator (enter '=' to finish, 'q' to quit)\n");
    printf("Current result: 0\n");
    
    while (state != DONE && state != ERROR) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            state = ERROR;
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        for (pos = 0; pos < len; pos++) {
            char c = input[pos];
            
            switch (state) {
                case INIT:
                    if (c == 'q' || c == 'Q') {
                        state = DONE;
                        break;
                    } else if (isdigit(c) || c == '.' || c == '+' || c == '-') {
                        state = READING_NUM;
                        current_num = 0.0;
                        decimal_places = 0;
                        in_decimal = 0;
                        sign = 1;
                        pos--;
                    } else if (c == '=') {
                        state = DONE;
                    } else if (!isspace(c)) {
                        state = ERROR;
                    }
                    break;
                    
                case READING_NUM:
                    if (isdigit(c)) {
                        if (in_decimal) {
                            decimal_places++;
                            current_num = current_num * 10.0 + (c - '0');
                        } else {
                            current_num = current_num * 10.0 + (c - '0');
                        }
                    } else if (c == '.' && !in_decimal) {
                        in_decimal = 1;
                    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                        for (int i = 0; i < decimal_places; i++) {
                            current_num /= 10.0;
                        }
                        current_num *= sign;
                        
                        switch (current_op) {
                            case '+': result += current_num; break;
                            case '-': result -= current_num; break;
                            case '*': result *= current_num; break;
                            case '/': 
                                if (current_num == 0.0) {
                                    state = ERROR;
                                } else {
                                    result /= current_num;
                                }
                                break;
                        }
                        
                        if (state != ERROR) {
                            current_op = c;
                            state = READING_OP;
                        }
                    } else if (c == '=') {
                        for (int i = 0; i < decimal_places; i++) {
                            current_num /= 10.0;
                        }
                        current_num *= sign;
                        
                        switch (current_op) {
                            case '+': result += current_num; break;
                            case '-': result -= current_num; break;
                            case '*': result *= current_num; break;
                            case '/': 
                                if (current_num == 0.0) {
                                    state = ERROR;
                                } else {
                                    result /= current_num;
                                }
                                break;
                        }
                        state = DONE;
                    } else if (!isspace(c)) {
                        state = ERROR;
                    }
                    break;
                    
                case READING_OP:
                    if (isdigit(c) || c == '.' || c == '+' || c == '-') {
                        state = READING_NUM;
                        current_num = 0.0;
                        decimal_places = 0;
                        in_decimal = 0;
                        sign = 1;
                        pos--;
                    } else if (c == '=') {
                        state = DONE;
                    } else if (!isspace(c)) {
                        state = ERROR;
                    }
                    break;
                    
                case CALCULATING:
                    if (c == '=') {
                        state = DONE;
                    } else if (!isspace(c)) {
                        state = ERROR;
                    }
                    break;
                    
                case ERROR:
                case DONE:
                    break;
            }
            
            if (state == ERROR || state == DONE) {
                break;
            }
        }
        
        if (state == READING_OP) {
            printf("Current result: %g\n", result);
        }
    }
    
    if (state == ERROR) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Final result: %g\n", result);
    return 0;
}