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
    STATE_IDENTIFIER,
    STATE_NUMBER,
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
    {STATE_START, 'a', STATE_IDENTIFIER},
    {STATE_START, '0', STATE_NUMBER},
    {STATE_START, '+', STATE_OPERATOR},
    {STATE_START, ' ', STATE_START},
    {STATE_IDENTIFIER, 'a', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '0', STATE_IDENTIFIER},
    {STATE_IDENTIFIER, '+', STATE_OPERATOR},
    {STATE_IDENTIFIER, ' ', STATE_START},
    {STATE_NUMBER, '0', STATE_NUMBER},
    {STATE_NUMBER, '+', STATE_OPERATOR},
    {STATE_NUMBER, ' ', STATE_START},
    {STATE_OPERATOR, 'a', STATE_IDENTIFIER},
    {STATE_OPERATOR, '0', STATE_NUMBER},
    {STATE_OPERATOR, '+', STATE_OPERATOR},
    {STATE_OPERATOR, ' ', STATE_START}
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

void process_token(State state, char* token, int token_len) {
    if (token_len <= 0) return;
    
    token[token_len] = '\0';
    
    switch (state) {
        case STATE_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token);
            break;
        case STATE_NUMBER:
            printf("NUMBER: %s\n", token);
            break;
        case STATE_OPERATOR:
            printf("OPERATOR: %s\n", token);
            break;
        default:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char token[MAX_INPUT_LEN + 1];
    int token_len = 0;
    
    printf("Enter input string (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    State current_state = STATE_START;
    State prev_state = STATE_START;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (!isalnum(c) && c != '+' && c != ' ') {
            fprintf(stderr, "Invalid character: %c\n", c);
            return 1;
        }
        
        State next_state = get_next_state(current_state, c);
        
        if (next_state == STATE_ERROR) {
            fprintf(stderr, "Invalid transition from state %d with char %c\n", 
                    current_state, c);
            return 1;
        }
        
        if (current_state != STATE_START && current_state != next_state) {
            process_token(current_state, token, token_len);
            token_len = 0;
        }
        
        if (next_state != STATE_START && c != ' ') {
            if (token_len < MAX_INPUT_LEN) {
                token[token_len++] = c;
            } else {
                fprintf(stderr, "Token too long\n");
                return 1;
            }
        }
        
        prev_state = current_state;
        current_state = next_state;
    }
    
    if (current_state != STATE_START) {
        process_token(current_state, token, token_len);
    }
    
    return 0;
}