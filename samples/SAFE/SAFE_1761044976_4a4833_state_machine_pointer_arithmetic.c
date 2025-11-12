//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
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
    EVENT_WHITESPACE,
    EVENT_END,
    EVENT_INVALID
} Event;

State transition_table[MAX_STATES][6] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_OPERATOR, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace((unsigned char)c)) return EVENT_WHITESPACE;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (strchr("+-*/=<>!&|", c) != NULL) return EVENT_OPERATOR;
    return EVENT_INVALID;
}

void process_token(char *start, char *end, State state) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT_LEN) return;
    
    char token[MAX_INPUT_LEN];
    char *dest = token;
    char *src = start;
    
    while (src < end && (size_t)(dest - token) < sizeof(token) - 1) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    
    const char *type_names[] = {"START", "IDENTIFIER", "NUMBER", "OPERATOR", "END", "ERROR"};
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("Token: '%-20s' Type: %s\n", token, type_names[state]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input)) {
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char *token_start = input;
    char *current_pos = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_event(*current_pos);
        
        if (event == EVENT_INVALID) {
            current_state = STATE_ERROR;
            break;
        }
        
        previous_state = current_state;
        current_state = transition_table[current_state][event];
        
        if (previous_state != current_state && previous_state != STATE_START) {
            process_token(token_start, current_pos, previous_state);
            token_start = current_pos;
        }
        
        if (current_state == STATE_START) {
            token_start = current_pos + 1;
        }
        
        if (*current_pos == '\0') break;
        current_pos++;
        
        if ((size_t)(current_pos - input) >= sizeof(input)) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input at position %ld\n", (long)(current_pos - input));
        return EXIT_FAILURE;
    }
    
    if (previous_state != STATE_START && previous_state != STATE_END) {
        process_token(token_start, current_pos, previous_state);
    }
    
    return EXIT_SUCCESS;
}