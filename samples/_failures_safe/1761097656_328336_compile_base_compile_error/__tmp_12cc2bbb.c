//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_START,
    STATE_NUMBER,
    STATE_IDENTIFIER,
    STATE_OPERATOR,
    STATE_ERROR,
    STATE_END
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

Transition transitions[] = {
    {STATE_START, '0', STATE_NUMBER},
    {STATE_START, '1', STATE_NUMBER},
    {STATE_START, '2', STATE_NUMBER},
    {STATE_START, '3', STATE_NUMBER},
    {STATE_START, '4', STATE_NUMBER},
    {STATE_START, '5', STATE_NUMBER},
    {STATE_START, '6', STATE_NUMBER},
    {STATE_START, '7', STATE_NUMBER},
    {STATE_START, '8', STATE_NUMBER},
    {STATE_START, '9', STATE_NUMBER},
    {STATE_START, 'a', STATE_IDENTIFIER},
    {STATE_START, 'b', STATE_IDENTIFIER},
    {STATE_START, 'c', STATE_IDENTIFIER},
    {STATE_START, '+', STATE_OPERATOR},
    {STATE_START, '-', STATE_OPERATOR},
    {STATE_START, '*', STATE_OPERATOR},
    {STATE_START, '/', STATE_OPERATOR},
    {STATE_NUMBER, '0', STATE_NUMBER},
    {STATE_NUMBER, '1', STATE_NUMBER},
    {STATE_NUMBER, '2', STATE_NUMBER},
    {STATE_NUMBER, '3', STATE_NUMBER},
    {STATE_NUMBER, '4', STATE_NUMBER},
    {STATE_NUMBER, '5', STATE_NUMBER},
    {STATE_NUMBER, '6', STATE_NUMBER},
    {STATE_NUMBER, '7', STATE_NUMBER},
    {STATE_NUMBER, '8', STATE_NUMBER},
    {STATE_NUMBER, '9', STATE_NUMBER},
    {STATE_IDENTIFIER, 'a', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, 'b', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, 'c', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '0', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '1', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '2', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '3', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '4', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '5', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '6', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '7', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '8', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '9', STATE_IDENTIFIER}
};

const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_START: return "START";
        case STATE_NUMBER: return "NUMBER";
        case STATE_IDENTIFIER: return "IDENTIFIER";
        case STATE_OPERATOR: return "OPERATOR";
        case STATE_ERROR: return "ERROR";
        case STATE_END: return "END";
        default: return "UNKNOWN";
    }
}

int is_valid_input_char(char c) {
    return (c >= '0' && c <= '9') || 
           (c >= 'a' && c <= 'c') ||
           c == '+' || c == '-' || c == '*' || c == '/' ||
           c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

void process_input(const char* input) {
    State current_state = STATE_START;
    int input_len;
    
    if (input == NULL) {
        printf("Input is NULL\n");
        return;
    }
    
    input_len = strlen(input);
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (int i = 0; i <= input_len; i++) {
        char current_char = input[i];
        
        if (!is_valid_input_char(current_char)) {
            printf("Invalid character: %c\n", current_char);
            current_state = STATE_ERROR;
            break;
        }
        
        if (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\0') {
            if (current_state != STATE_START && current_state != STATE_ERROR) {
                printf("Token: %s\n", state_to_string(current_state));
                current_state = STATE_START;
            }
            if (current_char == '\0') break;
            continue;
        }
        
        State next_state = get_next_state(current_state, current_char);
        
        if (