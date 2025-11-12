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
    {STATE_WHITESPACE, ' ', STATE_WHITESPACE},
    {STATE_WHITESPACE, '\t', STATE_WHITESPACE},
    {STATE_WHITESPACE, '\n', STATE_WHITESPACE},
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
    "START", "IDENTIFIER", "NUMBER", "OPERATOR", 
    "WHITESPACE", "ERROR", "END", "UNKNOWN"
};

State find_next_state(State current, char input) {
    Transition* trans_ptr = transitions;
    Transition* end_ptr = transitions + MAX_TRANSITIONS;
    
    while (trans_ptr < end_ptr) {
        if (trans_ptr->current == current && trans_ptr->trigger == input) {
            return trans_ptr->next;
        }
        trans_ptr++;
    }
    
    if (isalnum((unsigned char)input) || input == ' ' || input == '\t' || input == '\n' || 
        input == '+' || input == '-' || input == '*' || input == '/') {
        return STATE_UNKNOWN;
    }
    
    return STATE_ERROR;
}

void process_input(const char* input) {
    if (input == NULL) {
        return;
    }
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        return;
    }
    
    State current_state = STATE_START;
    const char* ptr = input;
    const char* end_ptr = input + len;
    
    printf("Input: %s\n", input);
    printf("State transitions:\n");
    
    while (ptr < end_ptr && current_state != STATE_ERROR && current_state != STATE_END) {
        State next_state = find_next_state(current_state, *ptr);
        
        printf("  '%c': %s -> %s\n", 
               *ptr, 
               state_names[current_state], 
               state_names[next_state]);
        
        current_state = next_state;
        ptr++;
        
        if (current_state == STATE_UNKNOWN) {
            current_state = STATE_ERROR