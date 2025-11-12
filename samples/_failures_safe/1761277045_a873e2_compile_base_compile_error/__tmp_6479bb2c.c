//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_START,
    STATE_NUMBER,
    STATE_IDENTIFIER,
    STATE_OPERATOR,
    STATE_END,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char trigger;
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
    {STATE_IDENTIFIER, '9', STATE_IDENTIFIER},
    {STATE_OPERATOR, ' ', STATE_END},
    {STATE_NUMBER, ' ', STATE_END},
    {STATE_IDENTIFIER, ' ', STATE_END}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State process_input(const char* input) {
    State current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input[i];
        int transition_found = 0;
        
        for (size_t j = 0; j < num_transitions; j++) {
            if (transitions[j].current == current_state && 
                transitions[j].trigger == current_char) {
                current_state = transitions[j].next;
                transition_found = 1;
                break;
            }
        }
        
        if (!transition_found) {
            return STATE_ERROR;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    return current_state;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_START: return "START";
        case STATE_NUMBER: return "NUMBER";
        case STATE_IDENTIFIER: return "IDENTIFIER";
        case STATE_OPERATOR: return "OPERATOR";
        case STATE_END: return "END";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    int valid_inputs = 0;
    int total_inputs = 0;
    
    printf("Enter expressions (empty line to quit):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (input[0] == '\0') {
            break;
        }
        
        if (strlen(input) >= MAX_INPUT_LEN - 1) {
            printf("Input too long