//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
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
    {STATE_START, ' ', STATE_START},
    {STATE_START, '\t', STATE_START},
    {STATE_START, '\n', STATE_START},
    {STATE_START, 'a', STATE_IDENTIFIER},
    {STATE_START, 'z', STATE_IDENTIFIER},
    {STATE_START, 'A', STATE_IDENTIFIER},
    {STATE_START, 'Z', STATE_IDENTIFIER},
    {STATE_START, '_', STATE_IDENTIFIER},
    {STATE_START, '0', STATE_NUMBER},
    {STATE_START, '9', STATE_NUMBER},
    {STATE_START, '+', STATE_OPERATOR},
    {STATE_START, '-', STATE_OPERATOR},
    {STATE_START, '*', STATE_OPERATOR},
    {STATE_START, '/', STATE_OPERATOR},
    {STATE_IDENTIFIER, 'a', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, 'z', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, 'A', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, 'Z', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '0', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '9', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '_', STATE_IDENTIFIER},
    {STATE_NUMBER, '0', STATE_NUMBER},
    {STATE_NUMBER, '9', STATE_NUMBER},
    {STATE_OPERATOR, ' ', STATE_START},
    {STATE_OPERATOR, '\t', STATE_START},
    {STATE_OPERATOR, '\n', STATE_START}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    Transition *t = transitions;
    Transition *end = t + num_transitions;
    
    while (t < end) {
        if (t->current == current && t->input == input) {
            return t->next;
        }
        t++;
    }
    
    if (current == STATE_IDENTIFIER && (isalnum(input) || input == '_')) {
        return STATE_IDENTIFIER;
    }
    if (current == STATE_NUMBER && isdigit(input)) {
        return STATE_NUMBER;
    }
    if (isspace(input)) {
        return STATE_START;
    }
    
    return STATE_ERROR;
}

void process_token(State state, char *start, char *end) {
    if (end > start) {
        size_t len = end - start;
        if (len >= MAX_INPUT_LEN) {
            return;
        }
        
        char token[MAX_INPUT_LEN];
        if (len < sizeof(token)) {
            memcpy(token, start, len);
            token[len] = '\0';
            
            switch (state) {
                case STATE_IDENTIFIER:
                    printf("Identifier: %s\n", token);
                    break;
                case STATE_NUMBER:
                    printf("Number: %s\n", token);
                    break;
                case STATE_OPERATOR:
                    printf("Operator: %s\n", token);
                    break;
                default:
                    break;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter input string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= sizeof(input)) {
        return 1;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char *token_start = input;
    char *ptr = input;
    char *end = input + input_len;
    
    while (ptr < end) {
        previous_state = current_state;
        current_state = get_next_state(current_state, *ptr);
        
        if (current_state != previous_state && previous_state != STATE_START && previous_state != STATE_ERROR) {
            process_token(previous_state, token_start, ptr);
            token_start = ptr;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Error: invalid character '%c'\n", *ptr);
            current_state = STATE_START;
            token_start = ptr + 1;
        }
        
        if (current_state == STATE_START && !isspace(*ptr)) {
            token_start = ptr;
        }
        
        ptr++;
    }
    
    if (current_state != STATE_START && current_state != STATE_ERROR) {
        process_token(current_state, token_start, ptr);
    }
    
    return 0;
}