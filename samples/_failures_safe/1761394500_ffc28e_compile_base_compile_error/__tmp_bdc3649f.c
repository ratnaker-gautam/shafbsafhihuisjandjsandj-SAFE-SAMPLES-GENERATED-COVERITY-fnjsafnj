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
    STATE_END,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char trigger;
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
    {STATE_NUMBER, '.', STATE_NUMBER},
    {STATE_OPERATOR, ' ', STATE_START},
    {STATE_OPERATOR, '\t', STATE_START},
    {STATE_OPERATOR, '\n', STATE_START}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State find_next_state(State current, char input) {
    Transition *t = transitions;
    Transition *end = t + num_transitions;
    
    while (t < end) {
        if (t->current == current && t->trigger == input) {
            return t->next;
        }
        t++;
    }
    
    if (current == STATE_IDENTIFIER && (isalnum(input) || input == '_')) {
        return STATE_IDENTIFIER;
    }
    if (current == STATE_NUMBER && (isdigit(input) || input == '.')) {
        return STATE_NUMBER;
    }
    if (isspace(input)) {
        return STATE_START;
    }
    
    return STATE_ERROR;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    const char *token_start = input;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        printf("Error: Input too long\n");
        return;
    }
    
    while (*ptr != '\0') {
        State next_state = find_next_state(current_state, *ptr);
        
        if (current_state != STATE_START && next_state == STATE_START) {
            size_t token_len = ptr - token_start;
            if (token_len > 0 && token_len < MAX_INPUT_LEN) {
                char token[MAX_INPUT_LEN];
                strncpy(token, token_start, token_len);
                token[token_len] = '\0';
                
                switch (current_state) {
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
            token_start = ptr + 1;
        }
        else if (current_state == STATE_START && next_state != STATE_START) {
            token_start = ptr;
        }
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid character '%c' at position %ld\n", *ptr, (long)(ptr - input));
            return;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state != STATE_START) {
        size_t token_len = ptr - token_start;
        if (token_len > 0 && token_len < MAX_INPUT_LEN) {
            char token[MAX_INPUT_LEN];
            strncpy(token, token_start, token_len);
            token[token_len] = '\0';
            
            switch (current_state) {
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

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter expression to tokenize: