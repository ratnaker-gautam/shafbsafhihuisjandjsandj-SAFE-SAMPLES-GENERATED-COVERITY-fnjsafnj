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
} parser_state_t;

typedef struct {
    parser_state_t from;
    char trigger;
    parser_state_t to;
} transition_t;

typedef struct {
    char input[MAX_INPUT_LEN + 1];
    size_t pos;
    parser_state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    size_t num_transitions;
} state_machine_t;

void initialize_transitions(state_machine_t *sm) {
    if (!sm) return;
    
    sm->num_transitions = 0;
    
    sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, ' ', STATE_START};
    sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, '\0', STATE_END};
    
    for (char c = 'a'; c <= 'z'; c++) {
        if (sm->num_transitions < MAX_TRANSITIONS - 2) {
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, c, STATE_IDENTIFIER};
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_IDENTIFIER, c, STATE_IDENTIFIER};
        }
    }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (sm->num_transitions < MAX_TRANSITIONS - 2) {
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, c, STATE_IDENTIFIER};
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_IDENTIFIER, c, STATE_IDENTIFIER};
        }
    }
    
    for (char c = '0'; c <= '9'; c++) {
        if (sm->num_transitions < MAX_TRANSITIONS - 3) {
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, c, STATE_NUMBER};
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_NUMBER, c, STATE_NUMBER};
            sm->transitions[sm->num_transitions++] = (transition_t){STATE_IDENTIFIER, c, STATE_IDENTIFIER};
        }
    }
    
    if (sm->num_transitions < MAX_TRANSITIONS - 4) {
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, '+', STATE_OPERATOR};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, '-', STATE_OPERATOR};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, '*', STATE_OPERATOR};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_START, '/', STATE_OPERATOR};
    }
    
    if (sm->num_transitions < MAX_TRANSITIONS - 3) {
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_IDENTIFIER, ' ', STATE_START};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_NUMBER, ' ', STATE_START};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_OPERATOR, ' ', STATE_START};
    }
    
    if (sm->num_transitions < MAX_TRANSITIONS - 3) {
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_IDENTIFIER, '\0', STATE_END};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_NUMBER, '\0', STATE_END};
        sm->transitions[sm->num_transitions++] = (transition_t){STATE_OPERATOR, '\0', STATE_END};
    }
}

void initialize_state_machine(state_machine_t *sm, const char *input) {
    if (!sm || !input) return;
    
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        len = MAX_INPUT_LEN;
    }
    
    strncpy(sm->input, input, len);
    sm->input[len] = '\0';
    sm->pos = 0;
    sm->current_state = STATE_START;
    initialize_transitions(sm);
}

parser_state_t get_next_state(state_machine_t *sm, char input_char) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].from == sm->current_state && 
            sm->transitions[i].trigger == input_char) {
            return sm->transitions[i].to;
        }
    }
    return STATE_ERROR;
}

int process_input(state_machine_t *sm) {
    if (!sm) return -1;
    
    size_t token_count = 0;
    
    while (sm->current_state != STATE_END && sm->current_state != STATE_ERROR) {
        if (sm->pos >= MAX_INPUT_LEN) {
            return -1;