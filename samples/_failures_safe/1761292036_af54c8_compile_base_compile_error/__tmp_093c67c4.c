//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUM,
    READING_OP,
    CALCULATING,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    double accumulator = 0.0;
    double current_num = 0.0;
    char current_op = '+';
    char input[256];
    int pos = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    int valid_input = 1;
    
    printf("Simple Calculator (enter '=' to finish, 'q' to quit)\n");
    printf("Supported operations: +, -, *, /\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (current_state == INIT) {
            printf("Enter number: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                current_state = ERROR;
                break;
            }
            
            if (strlen(input) > 0 && input[0] == 'q') {
                current_state = FINISHED;
                break;
            }
            
            pos = 0;
            current_num = 0.0;
            decimal_places = 0;
            in_decimal = 0;
            valid_input = 1;
            
            while (input[pos] != '\0' && input[pos] != '\n') {
                if (input[pos] == '.') {
                    if (in_decimal) {
                        valid_input = 0;
                        break;
                    }
                    in_decimal = 1;
                } else if (isdigit(input[pos])) {
                    if (in_decimal) {
                        decimal_places++;
                        current_num = current_num * 10.0 + (input[pos] - '0');
                    } else {
                        current_num = current_num * 10.0 + (input[pos] - '0');
                    }
                } else if (pos == 0 && (input[pos] == '+' || input[pos] == '-')) {
                } else {
                    valid_input = 0;
                    break;
                }
                pos++;
            }
            
            if (!valid_input) {
                printf("Invalid number format\n");
                current_state = INIT;
                continue;
            }
            
            while (decimal_places > 0) {
                current_num /= 10.0;
                decimal_places--;
            }
            
            if (pos > 0 && (input[0] == '-')) {
                current_num = -current_num;
            }
            
            accumulator = current_num;
            current_state = READING_OP;
        }
        
        if (current_state == READING_OP) {
            printf("Enter operation (+, -, *, /, =): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                current_state = ERROR;
                break;
            }
            
            if (strlen(input) > 0 && input[0] == 'q') {
                current_state = FINISHED;
                break;
            }
            
            if (strlen(input) >= 1) {
                char op = input[0];
                if (op == '=') {
                    current_state = FINISHED;
                } else if (op == '+' || op == '-' || op == '*' || op == '/') {
                    current_op = op;
                    current_state = READING_NUM;
                } else {
                    printf("Invalid operation\n");
                    current_state = READING_OP;
                }
            } else {
                current_state = READING_OP;
            }
        }
        
        if (current_state == READING_NUM) {
            printf("Enter number: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                current_state = ERROR;
                break;
            }
            
            if (strlen(input) > 0 && input[0] == 'q') {
                current_state = FINISHED;
                break;
            }
            
            pos = 0;
            current_num = 0.0;
            decimal_places = 0;
            in_decimal = 0;
            valid_input = 1;
            
            while (input[pos] != '\0' && input[pos] != '\n') {
                if (input[pos] == '.') {
                    if (in_decimal) {
                        valid_input = 0;
                        break;
                    }
                    in_decimal = 1;
                } else if (isdigit(input[pos])) {
                    if (in_decimal) {
                        decimal_places++;
                        current_num = current_num * 10.0 + (input[pos] - '0');
                    } else {
                        current_num = current_num * 10.0 + (input[pos] - '0');
                    }
                } else if (pos == 0 && (input[pos] == '+' || input[pos] == '-')) {
                } else {
                    valid_input = 0;
                    break;
                }
                pos++;
            }
            
            if (!valid_input) {
                printf("Invalid number format\n");
                current_state = READING_NUM;
                continue;
            }
            
            while (decimal_places > 0) {
                current_num /= 10.0;
                decimal_places--;
            }
            
            if (pos > 0 && (input[0] == '-')) {
                current_num = -current_num;
            }
            
            current_state = CALCULATING;
        }
        
        if (current_state == CALCULATING) {
            switch (current_op) {
                case '+':
                    accumulator += current_num;
                    break;
                case '-':
                    accumulator -= current_num;