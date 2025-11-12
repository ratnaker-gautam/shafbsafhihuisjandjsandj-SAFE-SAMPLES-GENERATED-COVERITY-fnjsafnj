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
    if (sm == NULL || input == NULL) return;
    sm->current_state = STATE_START;
    sm->input = input;
    sm->position = 0;
    sm->length = strlen(input);
}

int process_char(state_machine_t *sm) {
    if (sm == NULL || sm->input == NULL) return STATE_ERROR;
    if (sm->position >= sm->length) return STATE_ERROR;
    
    char current = *(sm->input + sm->position);
    
    switch (sm->current_state) {
        case STATE_START:
            if (isalpha(current)) {
                sm->current_state = STATE_IDENTIFIER;
            } else if (isdigit(current)) {
                sm->current_state = STATE_NUMBER;
            } else if (strchr("+-*/", current) != NULL) {
                sm->current_state = STATE_OPERATOR;
            } else if (isspace(current)) {
                sm->current_state = STATE_START;
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_IDENTIFIER:
            if (!isalnum(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_OPERATOR:
            sm->current_state = STATE_ERROR;
            break;
            
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
    
    return sm->current_state;
}

const char* state_to_string(int state) {
    switch (state) {
        case STATE_START: return "START";
        case STATE_IDENTIFIER: return "IDENTIFIER";
        case STATE_NUMBER: return "NUMBER";
        case STATE_OPERATOR: return "OPERATOR";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    char input_buffer[MAX_INPUT_LEN + 1];
    state_machine_t sm;
    
    printf("Enter expression to analyze: ");
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    init_state_machine(&sm, input_buffer);
    
    printf("Processing input: %s\n", input_buffer);
    printf("State transitions:\n");
    
    for (sm.position = 0; sm.position < sm.length; sm.position++) {
        int prev_state = sm.current_state;
        int new_state = process_char(&sm);
        
        printf("Char '%c': %s -> %s\n", 
               *(sm.input + sm.position),
               state_to_string(prev_state),
               state_to_string(new_state));
        
        if (new_state == STATE_ERROR) {
            printf("Invalid expression at position %zu\n", sm.position);
            break;
        }
    }
    
    if (sm.current_state != STATE_ERROR && sm.position == sm.length) {
        printf("Expression is valid\n");
    } else {
        printf("Expression is invalid\n");
    }
    
    return 0;
}