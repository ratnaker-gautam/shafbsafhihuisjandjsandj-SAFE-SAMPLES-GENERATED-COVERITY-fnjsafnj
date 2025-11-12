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
    STATE_UNKNOWN
} State;

typedef enum {
    EVENT_LETTER,
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
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event_type(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isalpha(c)) return EVENT_LETTER;
    if (isdigit(c)) return EVENT_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return EVENT_OPERATOR;
    if (isspace(c)) return EVENT_WHITESPACE;
    return EVENT_OTHER;
}

const char* state_to_string(State s) {
    static const char* names[] = {
        "START", "IDENTIFIER", "NUMBER", "OPERATOR", 
        "WHITESPACE", "ERROR", "END", "UNKNOWN"
    };
    return (s >= STATE_START && s <= STATE_UNKNOWN) ? names[s] : "INVALID";
}

void process_token(State state, char* start, char* end) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT_LEN) len = MAX_INPUT_LEN - 1;
    
    char token[MAX_INPUT_LEN];
    for (size_t i = 0; i < len && i < MAX_INPUT_LEN - 1; i++) {
        token[i] = *(start + i);
    }
    token[len] = '\0';
    
    printf("Token: '%-20s' Type: %s\n", token, state_to_string(state));
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("Empty input\n");
        return 0;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char* token_start = input;
    char* current_ptr = input;
    
    printf("Parsing: '%s'\n", input);
    printf("Tokens found:\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_event_type(*current_ptr);
        
        if (event < 0 || event > 5) {
            current_state = STATE_ERROR;
            break;
        }
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
            break;
        }
        
        previous_state = current_state;
        current_state = transition_table[current_state][event];
        
        if (current_state != previous_state && previous_state != STATE_START && previous_state != STATE_WHITESPACE) {
            process_token(previous_state, token_start, current_ptr);
            token_start = current_ptr;
        }
        
        if (current_state == STATE_START || current_state == STATE_WHITESPACE) {
            token_start = current_ptr + 1;
        }
        
        if (*current_ptr == '\0') break;
        current_ptr++;
        
        if (current_ptr - input >= (long)sizeof(input)) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input at position %ld\n", current_ptr - input);
        return 1;
    }
    
    if (previous_state != STATE_START && previous_state != STATE_WHITESPACE && previous_state != STATE_END) {
        process_token(previous_state, token_start, current_ptr);
    }
    
    printf("Parsing completed successfully\n