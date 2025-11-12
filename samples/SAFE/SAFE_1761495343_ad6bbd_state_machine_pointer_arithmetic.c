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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_OPERATOR:
            if (!is_operator_char(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            break;
    }
}

void print_token(state_machine_t *sm, int start_pos) {
    if (start_pos >= sm->position) return;
    
    size_t token_len = sm->position - start_pos;
    if (token_len >= MAX_INPUT_LEN) return;
    
    char token[MAX_INPUT_LEN];
    for (size_t i = 0; i < token_len; i++) {
        token[i] = *(sm->input + start_pos + i);
    }
    token[token_len] = '\0';
    
    const char *type_str;
    switch (sm->current_state) {
        case STATE_IDENTIFIER: type_str = "IDENTIFIER"; break;
        case STATE_NUMBER: type_str = "NUMBER"; break;
        case STATE_OPERATOR: type_str = "OPERATOR"; break;
        default: type_str = "ERROR"; break;
    }
    
    printf("%s: '%s'\n", type_str, token);
}

void process_input(state_machine_t *sm) {
    int token_start = -1;
    
    while (sm->position < sm->length) {
        char current = *(sm->input + sm->position);
        
        if (sm->current_state == STATE_START && !isspace(current)) {
            token_start = sm->position;
        }
        
        process_char(sm);
        
        if (sm->current_state == STATE_ERROR || isspace(current)) {
            if (token_start != -1 && sm->current_state != STATE_START) {
                print_token(sm, token_start);
            }
            
            sm->current_state = STATE_START;
            token_start = -1;
        }
        
        sm->position++;
    }
    
    if (token_start != -1 && sm->current_state != STATE_START) {
        print_token(sm, token_start);
    }
}

int main() {
    char input_buffer[MAX_INPUT_LEN];
    
    printf("Enter expression to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input_buffer[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    state_machine_t sm;
    init_state_machine(&sm, input_buffer);
    process_input(&sm);
    
    return 0;
}