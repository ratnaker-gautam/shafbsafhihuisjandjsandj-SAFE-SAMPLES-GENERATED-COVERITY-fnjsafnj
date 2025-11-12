//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 16
#define MAX_TRANSITIONS 64
#define MAX_INPUT_SIZE 1024

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H,
    NUM_STATES
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
    sm->state_count = NUM_STATES;
    sm->transition_count = 0;
    sm->current_state = STATE_A;
    
    for (int i = 0; i < NUM_STATES; i++) {
        sm->states[i] = (State)i;
    }
    
    Transition trans[] = {
        {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
        {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_A},
        {STATE_E, '0', STATE_B}, {STATE_E, '1', STATE_C},
        {STATE_F, '0', STATE_D}, {STATE_F, '1', STATE_E},
        {STATE_G, '0', STATE_F}, {STATE_G, '1', STATE_G},
        {STATE_H, '0', STATE_H}, {STATE_H, '1', STATE_A}
    };
    
    int trans_count = sizeof(trans) / sizeof(trans[0]);
    if (trans_count > MAX_TRANSITIONS) {
        trans_count = MAX_TRANSITIONS;
    }
    
    for (int i = 0; i < trans_count; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count] = trans[i];
            sm->transition_count++;
        }
    }
}

State process_input(StateMachine *sm, char input) {
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

void simulate_machine(StateMachine *sm, const char *input) {
    if (input == NULL) {
        return;
    }
    
    size_t len = strnlen(input, MAX_INPUT_SIZE);
    if (len == 0 || len >= MAX_INPUT_SIZE) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            continue;
        }
        
        State new_state = process_input(sm, input[i]);
        sm->current_state = new_state;
        
        for (int j = 0; j < 1000; j++) {
            int computation = (j * j) % 1000;
            computation = (computation * computation) % 1000;
        }
    }
}

const char *state_to_string(State state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input_buffer[MAX_INPUT_SIZE];
    
    printf("Enter binary sequence (0s and 1s, max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input_buffer, sizeof(input_buffer));
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    printf("Processing input...\n");
    
    simulate_machine(&sm, input_buffer);
    
    printf("Final state: %s\n", state_to_string(sm.current_state));
    
    return 0;
}