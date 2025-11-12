//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 256

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->state_count = 8;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    sm->states[5] = STATE_F;
    sm->states[6] = STATE_G;
    sm->states[7] = STATE_H;
    
    sm->transition_count = 16;
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_E};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_F};
    sm->transitions[5] = (Transition){STATE_C, '1', STATE_G};
    sm->transitions[6] = (Transition){STATE_D, '0', STATE_H};
    sm->transitions[7] = (Transition){STATE_D, '1', STATE_A};
    sm->transitions[8] = (Transition){STATE_E, '0', STATE_B};
    sm->transitions[9] = (Transition){STATE_E, '1', STATE_C};
    sm->transitions[10] = (Transition){STATE_F, '0', STATE_D};
    sm->transitions[11] = (Transition){STATE_F, '1', STATE_E};
    sm->transitions[12] = (Transition){STATE_G, '0', STATE_F};
    sm->transitions[13] = (Transition){STATE_G, '1', STATE_G};
    sm->transitions[14] = (Transition){STATE_H, '0', STATE_H};
    sm->transitions[15] = (Transition){STATE_H, '1', STATE_A};
    
    sm->current_state = STATE_A;
}

State process_transition(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_A;
    
    for (int i = 0; i < sm->transition_count && i < MAX_TRANSITIONS; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

void process_input_sequence(StateMachine *sm, const char *sequence, int length) {
    if (sm == NULL || sequence == NULL || length <= 0 || length > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < length; i++) {
        char input = sequence[i];
        if (input != '0' && input != '1') {
            continue;
        }
        sm->current_state = process_transition(sm, input);
    }
}

int validate_sequence(const char *sequence, int max_length) {
    if (sequence == NULL || max_length <= 0 || max_length > MAX_INPUTS) {
        return 0;
    }
    
    int length = 0;
    while (length < max_length && sequence[length] != '\0') {
        if (sequence[length] != '0' && sequence[length] != '1') {
            return 0;
        }
        length++;
        if (length >= MAX_INPUTS) {
            return 0;
        }
    }
    return length;
}

void generate_test_sequence(char *buffer, int length) {
    if (buffer == NULL || length <= 0 || length > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < length; i++) {
        buffer[i] = (rand() % 2) ? '1' : '0';
    }
    buffer[length] = '\0';
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    char test_sequence[MAX_INPUTS + 1];
    int sequence_length = 1000000;
    
    if (sequence_length > MAX_INPUTS) {
        sequence_length = MAX_INPUTS;
    }
    
    generate_test_sequence(test_sequence, sequence_length);
    
    int valid_length = validate_sequence(test_sequence, sequence_length);
    if (valid_length == 0) {
        printf("Invalid sequence generated\n");
        return 1;
    }
    
    printf("Processing %d state transitions...\n", valid_length);
    
    for (int iteration = 0; iteration < 100; iteration++) {
        sm.current_state = STATE