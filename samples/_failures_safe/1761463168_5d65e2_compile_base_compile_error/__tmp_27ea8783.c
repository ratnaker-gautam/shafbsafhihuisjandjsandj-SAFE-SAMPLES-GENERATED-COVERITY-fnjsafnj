//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

const char* state_names[] = {
    "START",
    "NUMBER",
    "IDENTIFIER",
    "OPERATOR",
    "ERROR",
    "END"
};

State get_next_state(State current, char input) {
    size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && 
            input[i] != '+' && input[i] != '-' && 
            input[i] != '*' && input[i] != '/') {
            return 0;
        }
    }
    return 1;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        State next_state = get_next_state(current_state, input[i]);
        printf("Char: '%c' | State: %s -> %s\n", 
               input[i], 
               state_names[current_state],
               state_names[next_state]);
        current_state = next_state;
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input sequence at position %zu\n", i);
            return;
        }
    }
    
    if (current_state == STATE_NUMBER) {
        printf("Valid number sequence\n");
    } else if (current_state == STATE_IDENTIFIER) {
        printf("Valid identifier sequence\n");
    } else if (current_state == STATE_OPERATOR) {
        printf("Valid operator sequence\n");
    } else {
        printf("Incomplete sequence\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter input sequence (max %d chars, alphanumeric or +-*/): ", MAX_INPUT_LEN);
    
    if (fgets