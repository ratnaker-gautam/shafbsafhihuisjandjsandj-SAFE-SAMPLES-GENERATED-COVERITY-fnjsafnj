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

void state_machine_init(state_machine_t *sm, char *input) {
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
                if (isspace(current) || is_operator_char(current)) {
                    sm->current_state = STATE_START;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(current)) {
                if (current == '.' && isdigit(*(sm->input + sm->position + 1))) {
                } else if (isspace(current) || is_operator_char(current)) {
                    sm->current_state = STATE_START;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_OPERATOR:
            if (!is_operator_char(current)) {
                if (isspace(current) || isalpha(current) || isdigit(current)) {
                    sm->current_state = STATE_START;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_ERROR:
            if (isspace(current)) {
                sm->current_state = STATE_START;
            }
            break;
    }
}

void print_state_name(int state) {
    const char *names[] = {
        "START", "IDENTIFIER", "NUMBER", "OPERATOR", "ERROR"
    };
    if (state >= 0 && state <= 4) {
        printf("%s", names[state]);
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    state_machine_t sm;
    
    printf("Enter expression to analyze: ");
    if (fgets(input_buffer, MAX_INPUT_LEN + 1, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    state_machine_init(&sm, input_buffer);
    
    printf("State transitions:\n");
    printf("Position\tChar\tState\n");
    
    while (sm.position < sm.length) {
        char current_char = *(sm.input + sm.position);
        printf("%zu\t\t'%c'\t", sm.position, current_char);
        print_state_name(sm.current_state);
        printf("\n");
        
        process_char(&sm);
        sm.position++;
        
        if (sm.position < sm.length) {
            printf("%zu\t\t'%c'\t", sm.position, *(sm.input + sm.position));
            print_state_name(sm.current_state);
            printf("\n");
        }
    }
    
    printf("Final state: ");
    print_state_name(sm.current_state);
    printf("\n");
    
    return 0;
}