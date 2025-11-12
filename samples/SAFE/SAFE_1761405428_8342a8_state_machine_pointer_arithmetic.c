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

typedef enum {
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OPERATOR,
    EVENT_WHITESPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} event_t;

static event_t get_event(char c) {
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/') return EVENT_OPERATOR;
    if (isspace(c)) return EVENT_WHITESPACE;
    if (c == '\0') return EVENT_TERMINATOR;
    return EVENT_INVALID;
}

static state_t transition_table[MAX_STATES][6] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_NUMBER, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_WHITESPACE, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

static const char* state_names[MAX_STATES] = {
    "START", "IDENTIFIER", "NUMBER", "OPERATOR", "WHITESPACE", "ERROR", "END", "UNKNOWN"
};

static void process_token(char* start, char* end, state_t state) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT) len = MAX_INPUT - 1;
    
    char token[MAX_INPUT];
    for (size_t i = 0; i < len && i < MAX_INPUT - 1; i++) {
        token[i] = *(start + i);
    }
    token[len] = '\0';
    
    printf("Token: '%-20s' Type: %s\n", token, state_names[state]);
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter expression to tokenize: ");
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
    
    state_t current_state = STATE_START;
    state_t previous_state = STATE_START;
    char* current_pos = input;
    char* token_start = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        event_t event = get_event(*current_pos);
        
        if (event == EVENT_INVALID && *current_pos != '\0') {
            current_state = STATE_ERROR;
            break;
        }
        
        previous_state = current_state;
        current_state = transition_table[current_state][event];
        
        if (current_state == STATE_ERROR) {
            break;
        }
        
        if (previous_state != current_state && previous_state != STATE_START && previous_state != STATE_WHITESPACE) {
            process_token(token_start, current_pos, previous_state);
            token_start = current_pos;
        }
        
        if (previous_state == STATE_START || previous_state == STATE_WHITESPACE) {
            token_start = current_pos;
        }
        
        if (current_state == STATE_END) {
            if (previous_state != STATE_START && previous_state != STATE_WHITESPACE) {
                process_token(token_start, current_pos, previous_state);
            }
            break;
        }
        
        if (*current_pos == '\0') {
            if (previous_state != STATE_START && previous_state != STATE_WHITESPACE) {
                process_token(token_start, current_pos, previous_state);
            }
            break;
        }
        
        current_pos++;
        
        if (current_pos - input >= (long)sizeof(input)) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid character '%c' at position %ld\n", 
               *current_pos, current_pos - input + 1);
        return 1;
    }
    
    printf("Tokenization completed successfully\n");
    return 0;
}