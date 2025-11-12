//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    EVENT_LETTER,
    EVENT_DIGIT,
    EVENT_OPERATOR,
    EVENT_SPACE,
    EVENT_END,
    EVENT_INVALID
} Event;

State transition_table[MAX_STATES][6] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_OPERATOR, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_LETTER;
    if (isdigit(c)) return EVENT_DIGIT;
    if (strchr("+-*/=", c) != NULL) return EVENT_OPERATOR;
    return EVENT_INVALID;
}

void process_token(char *start, char *end, State state) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT_LEN) return;
    
    char token[MAX_INPUT_LEN];
    char *token_ptr = token;
    char *input_ptr = start;
    
    while (input_ptr < end && (token_ptr - token) < MAX_INPUT_LEN - 1) {
        *token_ptr = *input_ptr;
        token_ptr++;
        input_ptr++;
    }
    *token_ptr = '\0';
    
    const char *type_names[] = {"START", "IDENTIFIER", "NUMBER", "OPERATOR", "END", "ERROR"};
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("Token: '%-20s' Type: %s\n", token, type_names[state]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *token_start = input;
    char *current_ptr = input;
    
    while (*current_ptr != '\0' && current_state != STATE_ERROR) {
        Event event = get_event(*current_ptr);
        
        if (event < 0 || event > 5) {
            current_state = STATE_ERROR;
            break;
        }
        
        State next_state = transition_table[current_state][event];
        
        if (next_state != current_state) {
            if (current_state != STATE_START && current_state != STATE_END) {
                process_token(token_start, current_ptr, current_state);
            }
            token_start = current_ptr;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_ERROR) {
            break;
        }
        
        current_ptr++;
        
        if (current_ptr - input >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state != STATE_START && current_state != STATE_ERROR) {
        process_token(token_start, current_ptr, current_state);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid expression\n");
        return 1;
    }
    
    return 0;
}