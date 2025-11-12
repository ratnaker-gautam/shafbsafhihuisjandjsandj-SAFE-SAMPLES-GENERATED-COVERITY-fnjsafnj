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
} State;

typedef enum {
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OPERATOR,
    EVENT_WHITESPACE,
    EVENT_TERMINATOR,
    EVENT_OTHER
} Event;

State transition_table[MAX_STATES][6] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END, STATE_ERROR}
};

Event get_event_type(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return EVENT_OPERATOR;
    if (isspace(c)) return EVENT_WHITESPACE;
    return EVENT_OTHER;
}

void process_token(char *start, char *end, State state) {
    if (start >= end) return;
    
    size_t length = end - start;
    if (length >= MAX_INPUT) return;
    
    char token[MAX_INPUT];
    char *dest = token;
    char *src = start;
    
    while (src < end && dest < token + MAX_INPUT - 1) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    
    const char *type_str = "UNKNOWN";
    switch (state) {
        case STATE_IDENTIFIER: type_str = "IDENTIFIER"; break;
        case STATE_NUMBER: type_str = "NUMBER"; break;
        case STATE_OPERATOR: type_str = "OPERATOR"; break;
        case STATE_WHITESPACE: type_str = "WHITESPACE"; break;
        case STATE_ERROR: type_str = "ERROR"; break;
        default: type_str = "UNKNOWN"; break;
    }
    
    if (state != STATE_WHITESPACE && state != STATE_ERROR) {
        printf("Token: '%.*s' Type: %s\n", (int)length, start, type_str);
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char *current_pos = input;
    char *token_start = input;
    
    printf("Parsing: %s\n", input);
    
    while (current_state != STATE_END) {
        Event event = get_event_type(*current_pos);
        
        if (event >= 6 || current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
        } else {
            previous_state = current_state;
            current_state = transition_table[current_state][event];
        }
        
        if (previous_state != current_state) {
            process_token(token_start, current_pos, previous_state);
            token_start = current_pos;
        }
        
        if (current_state == STATE_END) {
            break;
        }
        
        if (*current_pos == '\0') {
            break;
        }
        
        current_pos++;
        if (current_pos >= input + MAX_INPUT) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input at position %ld\n", current_pos - input);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}