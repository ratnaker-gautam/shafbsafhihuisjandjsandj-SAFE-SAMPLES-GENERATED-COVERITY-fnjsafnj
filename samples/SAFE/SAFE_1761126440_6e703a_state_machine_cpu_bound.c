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
    
    int count = sizeof(ts) / sizeof(ts[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count] = ts[i];
            sm->transition_count++;
        }
    }
}

int process_input(StateMachine *sm, char input) {
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void simulate_machine(StateMachine *sm, const char *inputs, int input_len) {
    if (inputs == NULL || input_len <= 0 || input_len > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < input_len; i++) {
        char input_char = inputs[i];
        if (input_char != '0' && input_char != '1') {
            continue;
        }
        
        if (!process_input(sm, input_char)) {
            break;
        }
        
        for (int j = 0; j < 1000; j++) {
            volatile int computation = j * j + j;
            (void)computation;
        }
    }
}

const char* state_to_string(State s) {
    switch (s) {
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
    
    char input_buffer[MAX_INPUTS + 1];
    printf("Enter binary sequence (max %d chars, 0s and 1s only): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    printf("Processing %zu inputs...\n", len);
    
    simulate_machine(&sm, input_buffer, len);
    
    printf("Final state: %s\n", state_to_string(sm.current_state));
    
    return 0;
}