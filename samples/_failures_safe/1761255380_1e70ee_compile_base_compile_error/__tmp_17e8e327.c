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
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_READING_OPERATOR,
    STATE_ERROR,
    STATE_END
} State;

typedef struct {
    State current_state;
    char input_char;
    State next_state;
} Transition;

Transition transitions[] = {
    {STATE_START, ' ', STATE_START},
    {STATE_START, '\t', STATE_START},
    {STATE_START, '\n', STATE_START},
    {STATE_START, '0', STATE_READING_NUMBER},
    {STATE_START, '1', STATE_READING_NUMBER},
    {STATE_START, '2', STATE_READING_NUMBER},
    {STATE_START, '3', STATE_READING_NUMBER},
    {STATE_START, '4', STATE_READING_NUMBER},
    {STATE_START, '5', STATE_READING_NUMBER},
    {STATE_START, '6', STATE_READING_NUMBER},
    {STATE_START, '7', STATE_READING_NUMBER},
    {STATE_START, '8', STATE_READING_NUMBER},
    {STATE_START, '9', STATE_READING_NUMBER},
    {STATE_START, '+', STATE_READING_OPERATOR},
    {STATE_START, '-', STATE_READING_OPERATOR},
    {STATE_START, '*', STATE_READING_OPERATOR},
    {STATE_START, '/', STATE_READING_OPERATOR},
    {STATE_START, 'a', STATE_READING_WORD},
    {STATE_START, 'b', STATE_READING_WORD},
    {STATE_START, 'c', STATE_READING_WORD},
    {STATE_READING_NUMBER, '0', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '1', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '2', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '3', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '4', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '5', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '6', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '7', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '8', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '9', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, ' ', STATE_START},
    {STATE_READING_NUMBER, '\t', STATE_START},
    {STATE_READING_NUMBER, '\n', STATE_END},
    {STATE_READING_WORD, 'a', STATE_READING_WORD},
    {STATE_READING_WORD, 'b', STATE_READING_WORD},
    {STATE_READING_WORD, 'c', STATE_READING_WORD},
    {STATE_READING_WORD, ' ', STATE_START},
    {STATE_READING_WORD, '\t', STATE_START},
    {STATE_READING_WORD, '\n', STATE_END},
    {STATE_READING_OPERATOR, ' ', STATE_START},
    {STATE_READING_OPERATOR, '\t', STATE_START},
    {STATE_READING_OPERATOR, '\n', STATE_END}
};

#define NUM_TRANSITIONS (sizeof(transitions)/sizeof(transitions[0]))

State get_next_state(State current, char input) {
    for (size_t i = 0; i < NUM_TRANSITIONS; i++) {
        if (transitions[i].current_state == current && transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (!isprint(c) && c != '\n' && c != '\t') {
            printf("Error: Invalid character at position %zu\n", i);
            return;
        }
        
        current_state = get_next_state(current_state, c);
        
        if (current_state == STATE_ERROR) {
            printf("Error: Invalid transition at position %zu\n", i);
            return;
        }
        
        if (current_state == STATE_END) {
            if (i == input_len - 1) {
                printf("Success: Valid input\n");
                return;
            } else {
                printf("Error: Input continues after end state\n");
                return;
            }
        }
    }
    
    if (current_state != STATE_END) {
        printf("Error: Input ended in state %d\n", current_state);
    } else {
        printf("Success: Valid input\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];