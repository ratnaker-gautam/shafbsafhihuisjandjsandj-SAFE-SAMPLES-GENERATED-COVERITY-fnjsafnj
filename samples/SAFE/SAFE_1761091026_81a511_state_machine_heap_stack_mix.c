//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_SIZE 100

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    State *valid_states;
    int state_count;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->valid_states = malloc(MAX_STATES * sizeof(State));
    if (!sm->valid_states) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->valid_states[0] = STATE_A;
    sm->valid_states[1] = STATE_B;
    sm->valid_states[2] = STATE_C;
    sm->valid_states[3] = STATE_D;
    sm->valid_states[4] = STATE_E;
    sm->state_count = 5;
    
    sm->transition_count = 0;
    sm->current_state = STATE_A;
    
    Transition transitions[] = {
        {STATE_A, '1', STATE_B},
        {STATE_A, '2', STATE_C},
        {STATE_B, '3', STATE_D},
        {STATE_B, '4', STATE_E},
        {STATE_C, '5', STATE_D},
        {STATE_C, '6', STATE_E},
        {STATE_D, '7', STATE_A},
        {STATE_E, '8', STATE_A}
    };
    
    for (int i = 0; i < 8 && sm->transition_count < MAX_TRANSITIONS; i++) {
        sm->transitions[sm->transition_count++] = transitions[i];
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->valid_states);
        free(sm);
    }
}

int is_valid_state(StateMachine *sm, State state) {
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->valid_states[i] == state) {
            return 1;
        }
    }
    return 0;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            State next_state = sm->transitions[i].next;
            if (is_valid_state(sm, next_state)) {
                sm->current_state = next_state;
                return next_state;
            }
        }
    }
    return sm->current_state;
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_SIZE];
    
    printf("State Machine Simulation\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    printf("Valid inputs: 1,2,3,4,5,6,7,8\n");
    printf("Enter 'q' to quit\n");
    
    while (1) {
        printf("\nEnter input: ");
        
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        char input = input_buffer[0];
        
        if (input == 'q' || input == 'Q') {
            break;
        }
        
        State previous_state = sm->current_state;
        State new_state = process_input(sm, input);
        
        if (previous_state != new_state) {
            printf("Transition: %s -> %s\n", 
                   state_to_string(previous_state), 
                   state_to_string(new_state));
        } else {
            printf("No transition for input '%c' in state %s\n", 
                   input, state_to_string(previous_state));
        }
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    destroy_state_machine(sm);
    
    return 0;
}