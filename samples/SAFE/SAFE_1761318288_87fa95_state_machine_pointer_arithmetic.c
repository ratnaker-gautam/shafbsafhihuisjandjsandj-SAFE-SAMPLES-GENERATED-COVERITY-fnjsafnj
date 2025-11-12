//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define STATE_START 0
#define STATE_IDENTIFIER 1
#define STATE_NUMBER 2
#define STATE_OPERATOR 3
#define STATE_ERROR 4

typedef struct {
    int current_state;
    char *input;
    size_t position;
    size_t length;
} state_machine_t;

void init_state_machine(state_machine_t *sm, char *input) {
    sm->current_state = STATE_START;
    sm->input = input;
    sm->position = 0;
    sm->length = strlen(input);
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void process_char(state_machine_t *sm) {
    char current = *(sm->input + sm->position);
    
    switch (sm->current_state) {
        case STATE_START:
            if (isalpha(current) || current == '_') {
                sm->current_state = STATE_IDENTIFIER;
            } else if (isdigit(current)) {
                sm->current_state = STATE_NUMBER;
            } else if (is_operator_char(current)) {
                sm->current_state = STATE_OPERATOR;
            } else if (!isspace(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_IDENTIFIER:
            if (!is_valid_identifier_char(current)) {
                if (is_operator_char(current)) {
                    sm->current_state = STATE_OPERATOR;
                } else if (isdigit(current)) {
                    sm->current_state = STATE_NUMBER;
                } else if (!isspace(current)) {
                    sm->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(current)) {
                if (is_operator_char(current)) {
                    sm->current_state = STATE_OPERATOR;
                } else if (isalpha(current) || current == '_') {
                    sm->current_state = STATE_IDENTIFIER;
                } else if (!isspace(current)) {
                    sm->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_OPERATOR:
            if (!is_operator_char(current)) {
                if (isalpha(current) || current == '_') {
                    sm->current_state = STATE_IDENTIFIER;
                } else if (isdigit(current)) {
                    sm->current_state = STATE_NUMBER;
                } else if (!isspace(current)) {
                    sm->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_ERROR:
            break;
    }
}

void print_state_name(int state) {
    const char *names[] = {
        "START", "IDENTIFIER", "NUMBER", "OPERATOR", "ERROR"
    };
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("%s", names[state]);
    }
}

int main() {
    char input_buffer[MAX_INPUT_LEN + 1];
    state_machine_t sm;
    
    printf("Enter an expression to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_state_machine(&sm, input_buffer);
    
    printf("State transitions:\n");
    printf("Position 0: ");
    print_state_name(sm.current_state);
    printf("\n");
    
    for (sm.position = 0; sm.position < sm.length; sm.position++) {
        process_char(&sm);
        printf("Position %zu: ", sm.position + 1);
        print_state_name(sm.current_state);
        printf(" (char: '%c')\n", *(sm.input + sm.position));
        
        if (sm.current_state == STATE_ERROR) {
            printf("Invalid character sequence detected\n");
            break;
        }
    }
    
    printf("Final state: ");
    print_state_name(sm.current_state);
    printf("\n");
    
    return 0;
}