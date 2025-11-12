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
    STATE_END,
    STATE_ERROR
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
    {STATE_IDENTIFIER, '9', STATE_IDENTIFIER},
    {STATE_OPERATOR, ' ', STATE_END},
    {STATE_NUMBER, ' ', STATE_END},
    {STATE_IDENTIFIER, ' ', STATE_END}
};

const char* state_names[] = {
    "START",
    "NUMBER",
    "IDENTIFIER",
    "OPERATOR",
    "END",
    "ERROR"
};

State find_next_state(State current, char input) {
    size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_input(const char* input) {
    State current_state = STATE_START;
    size_t len = strlen(input);
    
    if (len == 0 || len > MAX_INPUT_LEN) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        if (c == '\0') break;
        
        current_state = find_next_state(current_state, c);
        if (current_state == STATE_ERROR) {
            return -1;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current_state != STATE_END && current_state != STATE_NUMBER && 
        current_state != STATE_IDENTIFIER && current_state != STATE_OPERATOR) {
        return -1;
    }
    
    return 0;
}

void run_state_machine_simulation() {
    char input[MAX_INPUT_LEN + 1];
    int valid_count = 0;
    int total_count = 0;
    
    const char* test_inputs[] = {
        "123 ",
        "abc ",
        "+ ",
        "123abc",
        "xyz",
        "123",
        "a1b2c3 ",
        "* ",
        "999 ",
        "abc123 "
    };
    
    size_t num_tests = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        strncpy(input, test_inputs[i], MAX_INPUT_LEN);