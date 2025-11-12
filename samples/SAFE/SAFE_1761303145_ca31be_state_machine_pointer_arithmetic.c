//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_WHITESPACE,
    STATE_ERROR,
    STATE_END,
    STATE_INVALID
} state_t;

typedef struct {
    state_t current;
    char *input;
    size_t pos;
    size_t len;
} state_machine_t;

state_t transition(state_machine_t *sm) {
    char current_char = *(sm->input + sm->pos);
    
    if (sm->pos >= sm->len) {
        return STATE_END;
    }
    
    switch (sm->current) {
        case STATE_START:
            if (isalpha(current_char)) return STATE_IDENTIFIER;
            if (isdigit(current_char)) return STATE_NUMBER;
            if (strchr("+-*/", current_char)) return STATE_OPERATOR;
            if (isspace(current_char)) return STATE_WHITESPACE;
            return STATE_ERROR;
            
        case STATE_IDENTIFIER:
            if (isalnum(current_char)) return STATE_IDENTIFIER;
            if (strchr("+-*/", current_char)) return STATE_OPERATOR;
            if (isspace(current_char)) return STATE_WHITESPACE;
            if (sm->pos >= sm->len) return STATE_END;
            return STATE_ERROR;
            
        case STATE_NUMBER:
            if (isdigit(current_char)) return STATE_NUMBER;
            if (strchr("+-*/", current_char)) return STATE_OPERATOR;
            if (isspace(current_char)) return STATE_WHITESPACE;
            if (sm->pos >= sm->len) return STATE_END;
            return STATE_ERROR;
            
        case STATE_OPERATOR:
            if (isalpha(current_char)) return STATE_IDENTIFIER;
            if (isdigit(current_char)) return STATE_NUMBER;
            if (isspace(current_char)) return STATE_WHITESPACE;
            if (sm->pos >= sm->len) return STATE_END;
            return STATE_ERROR;
            
        case STATE_WHITESPACE:
            if (isalpha(current_char)) return STATE_IDENTIFIER;
            if (isdigit(current_char)) return STATE_NUMBER;
            if (strchr("+-*/", current_char)) return STATE_OPERATOR;
            if (isspace(current_char)) return STATE_WHITESPACE;
            if (sm->pos >= sm->len) return STATE_END;
            return STATE_ERROR;
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_INVALID;
    }
}

void process_token(state_machine_t *sm, state_t prev_state) {
    size_t token_len = sm->pos;
    char *token_start = sm->input;
    
    if (prev_state != STATE_START && prev_state != STATE_WHITESPACE) {
        printf("Token: ");
        for (size_t i = 0; i < token_len; i++) {
            putchar(*(token_start + i));
        }
        printf(" | Type: ");
        
        switch (prev_state) {
            case STATE_IDENTIFIER: printf("IDENTIFIER\n"); break;
            case STATE_NUMBER: printf("NUMBER\n"); break;
            case STATE_OPERATOR: printf("OPERATOR\n"); break;
            case STATE_WHITESPACE: break;
            default: printf("UNKNOWN\n");
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    state_machine_t sm;
    
    printf("Enter expression: ");
    if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
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
    
    sm.input = input_buffer;
    sm.pos = 0;
    sm.len = input_len;
    sm.current = STATE_START;
    
    state_t prev_state = STATE_START;
    size_t start_pos = 0;
    
    while (sm.current != STATE_END && sm.current != STATE_ERROR && sm.current != STATE_INVALID) {
        prev_state = sm.current;
        sm.current = transition(&sm);
        
        if (sm.current != prev_state) {
            process_token(&sm, prev_state);
            start_pos = sm.pos;
        }
        
        if (sm.current != STATE_END) {
            sm.pos++;
        }
    }
    
    if (sm.current == STATE_ERROR || sm.current == STATE_INVALID) {
        fprintf(stderr, "Invalid input at position %zu\n", sm.pos);
        return 1;
    }
    
    process_token(&sm, sm.current);
    
    return 0;
}