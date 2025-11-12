//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4

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
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    
    Transition ts[] = {
        {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
        {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_A},
        {STATE_E, '0', STATE_B}, {STATE_E, '1', STATE_C},
        {STATE_F, '0', STATE_D}, {STATE_F, '1', STATE_E},
        {STATE_G, '0', STATE_F}, {STATE_G, '1', STATE_G},
        {STATE_H, '0', STATE_H}, {STATE_H, '1', STATE_A}
    };
    
    for (int i = 0; i < 16; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count++] = ts[i];
        }
    }
}

State process_input(StateMachine *sm, char input) {
    if (input != '0' && input != '1') {
        return sm->current_state;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    
    return sm->current_state;
}

void run_state_machine(StateMachine *sm, const char *input_sequence) {
    if (input_sequence == NULL) {
        return;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 1000) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        State next_state = process_input(sm, input_sequence[i]);
        sm->current_state = next_state;
        
        for (int j = 0; j < 1000; j++) {
            int computation = (j * j) % 100;
            computation = (computation * computation) % 100;
        }
    }
}

const char* state_to_string(State s) {
    switch(s) {
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

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input_buffer[1024];
    printf("Enter binary sequence (max 1000 chars, only 0/1): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
    }
    
    for (size_t i = 0; i < strlen(input_buffer); i++) {
        if (input_buffer[i] != '0' && input_buffer[i] != '1') {
            printf("Invalid input: only '0' and '1' allowed\n");
            return 1;
        }
    }
    
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    printf("Processing sequence...\n");
    
    run_state_machine(&sm, input_buffer);
    
    printf("Final state: %s\n", state_to_string(sm.current_state));
    
    return 0;
}