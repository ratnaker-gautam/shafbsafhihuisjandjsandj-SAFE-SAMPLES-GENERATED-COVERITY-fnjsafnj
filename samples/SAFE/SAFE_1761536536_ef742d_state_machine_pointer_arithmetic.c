//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_WHITESPACE,
    STATE_ERROR,
    STATE_END,
    STATE_UNKNOWN
} state_t;

typedef struct {
    state_t current;
    char *input;
    size_t pos;
    size_t len;
} context_t;

state_t transition(state_t current, char c) {
    switch (current) {
        case STATE_START:
            if (isalpha(c) || c == '_') return STATE_IDENTIFIER;
            if (isdigit(c)) return STATE_NUMBER;
            if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
            if (isspace(c)) return STATE_WHITESPACE;
            if (c == '\0') return STATE_END;
            return STATE_UNKNOWN;
        case STATE_IDENTIFIER:
            if (isalnum(c) || c == '_') return STATE_IDENTIFIER;
            if (isspace(c)) return STATE_WHITESPACE;
            if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
            if (c == '\0') return STATE_END;
            return STATE_UNKNOWN;
        case STATE_NUMBER:
            if (isdigit(c)) return STATE_NUMBER;
            if (isspace(c)) return STATE_WHITESPACE;
            if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
            if (c == '\0') return STATE_END;
            return STATE_UNKNOWN;
        case STATE_OPERATOR:
            if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
            if (isalpha(c) || c == '_') return STATE_IDENTIFIER;
            if (isdigit(c)) return STATE_NUMBER;
            if (isspace(c)) return STATE_WHITESPACE;
            if (c == '\0') return STATE_END;
            return STATE_UNKNOWN;
        case STATE_WHITESPACE:
            if (isspace(c)) return STATE_WHITESPACE;
            if (isalpha(c) || c == '_') return STATE_IDENTIFIER;
            if (isdigit(c)) return STATE_NUMBER;
            if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
            if (c == '\0') return STATE_END;
            return STATE_UNKNOWN;
        default:
            return STATE_ERROR;
    }
}

void process_token(state_t state, char *start, char *end) {
    if (start >= end) return;
    
    size_t length = end - start;
    if (length >= MAX_INPUT) return;
    
    char token[MAX_INPUT];
    strncpy(token, start, length);
    token[length] = '\0';
    
    const char *type_names[] = {
        "START", "IDENTIFIER", "NUMBER", "OPERATOR", 
        "WHITESPACE", "ERROR", "END", "UNKNOWN"
    };
    
    if (state < MAX_STATES) {
        printf("Token: '%-20s' Type: %s\n", token, type_names[state]);
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    context_t ctx;
    ctx.current = STATE_START;
    ctx.input = input;
    ctx.pos = 0;
    ctx.len = input_len;
    
    char *token_start = input;
    state_t prev_state = STATE_START;
    
    while (ctx.pos <= ctx.len) {
        char current_char = ctx.input[ctx.pos];
        state_t next_state = transition(ctx.current, current_char);
        
        if (next_state != ctx.current || next_state == STATE_END) {
            if (prev_state != STATE_WHITESPACE && prev_state != STATE_START) {
                process_token(prev_state, token_start, ctx.input + ctx.pos);
            }
            
            token_start = ctx.input + ctx.pos;
            prev_state = next_state;
        }
        
        ctx.current = next_state;
        ctx.pos++;
        
        if (next_state == STATE_END || next_state == STATE_ERROR) {
            break;
        }
    }
    
    if (prev_state != STATE_WHITESPACE && prev_state != STATE_START && prev_state != STATE_END) {
        process_token(prev_state, token_start, ctx.input + ctx.pos);
    }
    
    return EXIT_SUCCESS;
}