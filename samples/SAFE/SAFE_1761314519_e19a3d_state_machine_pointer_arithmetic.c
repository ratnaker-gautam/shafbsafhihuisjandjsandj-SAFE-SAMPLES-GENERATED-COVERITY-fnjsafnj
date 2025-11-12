//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 8
#define MAX_TRANSITIONS 16

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

typedef struct {
    State current;
    char trigger;
    State next;
} Transition;

Transition transitions[MAX_TRANSITIONS] = {
    {STATE_START, ' ', STATE_WHITESPACE},
    {STATE_START, '\t', STATE_WHITESPACE},
    {STATE_START, '\n', STATE_WHITESPACE},
    {STATE_START, '+', STATE_OPERATOR},
    {STATE_START, '-', STATE_OPERATOR},
    {STATE_START, '*', STATE_OPERATOR},
    {STATE_START, '/', STATE_OPERATOR},
    {STATE_START, '=', STATE_OPERATOR},
    {STATE_START, '0', STATE_NUMBER},
    {STATE_START, '1', STATE_NUMBER},
    {STATE_START, '2', STATE_NUMBER},
    {STATE_START, '3', STATE_NUMBER},
    {STATE_START, '4', STATE_NUMBER},
    {STATE_START, '5', STATE_NUMBER},
    {STATE_START, '6', STATE_NUMBER},
    {STATE_START, '7', STATE_NUMBER}
};

State get_next_state(State current, char input) {
    for (size_t i = 0; i < MAX_TRANSITIONS; i++) {
        Transition *t = transitions + i;
        if (t->current == current && t->trigger == input) {
            return t->next;
        }
    }
    
    if (current == STATE_START) {
        if (isalpha(input)) return STATE_IDENTIFIER;
        if (isdigit(input)) return STATE_NUMBER;
    }
    if (current == STATE_IDENTIFIER && (isalnum(input) || input == '_')) return STATE_IDENTIFIER;
    if (current == STATE_NUMBER && isdigit(input)) return STATE_NUMBER;
    if (current == STATE_WHITESPACE && isspace(input)) return STATE_WHITESPACE;
    
    return STATE_ERROR;
}

const char* state_name(State s) {
    static const char* names[] = {
        "START", "IDENTIFIER", "NUMBER", "OPERATOR", 
        "WHITESPACE", "ERROR", "END", "UNKNOWN"
    };
    if (s >= STATE_START && s <= STATE_UNKNOWN) {
        return names[s];
    }
    return "INVALID";
}

int main(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    State current_state = STATE_START;
    char *ptr = input;
    char *end = input + len;
    
    printf("State transitions:\n");
    
    while (ptr < end && *ptr != '\0') {
        State next_state = get_next_state(current_state, *ptr);
        
        printf("Char '%c': %s -> %s\n", 
               *ptr, state_name(current_state), state_name(next_state));
        
        current_state = next_state;
        ptr++;
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input sequence detected\n");
            break;
        }
    }
    
    if (current_state != STATE_ERROR) {
        printf("Final state: %s\n", state_name(current_state));
    }
    
    return 0;
}