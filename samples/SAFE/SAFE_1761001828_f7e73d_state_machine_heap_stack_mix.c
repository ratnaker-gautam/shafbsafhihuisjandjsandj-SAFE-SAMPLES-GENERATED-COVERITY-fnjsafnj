//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

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
    if (sm == NULL) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->valid_states = malloc(MAX_STATES * sizeof(State));
    if (sm->valid_states == NULL) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->state_count = 5;
    
    sm->valid_states[0] = STATE_A;
    sm->valid_states[1] = STATE_B;
    sm->valid_states[2] = STATE_C;
    sm->valid_states[3] = STATE_D;
    sm->valid_states[4] = STATE_E;
    
    return sm;
}

void add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) return;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
}

int is_valid_state(StateMachine *sm, State state) {
    if (sm == NULL) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->valid_states[i] == state) {
            return 1;
        }
    }
    return 0;
}

State process_input(StateMachine *sm, char input) {
    if (sm == NULL) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            if (is_valid_state(sm, sm->transitions[i].next)) {
                sm->current_state = sm->transitions[i].next;
                break;
            }
        }
    }
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    free(sm->transitions);
    free(sm->valid_states);
    free(sm);
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
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '1', STATE_C);
    add_transition(sm, STATE_B, '2', STATE_D);
    add_transition(sm, STATE_C, '1', STATE_D);
    add_transition(sm, STATE_C, '2', STATE_E);
    add_transition(sm, STATE_D, '1', STATE_E);
    add_transition(sm, STATE_D, '2', STATE_A);
    add_transition(sm, STATE_E, '1', STATE_A);
    add_transition(sm, STATE_E, '2', STATE_B);
    
    char input_buffer[MAX_INPUT_LEN];
    printf("State Machine Simulation (Enter 'q' to quit)\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    
    while (1) {
        printf("Enter input (1 or 2): ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0) continue;
        
        if (input_buffer[0] == 'q' || input_buffer[0] == 'Q') {
            break;
        }
        
        if (input_buffer[0] != '1' && input_buffer[0] != '2') {
            printf("Invalid input. Please enter 1 or 2.\n");
            continue;
        }
        
        State new_state = process_input(sm, input_buffer[0]);
        printf("New state: %s\n", state_to_string(new_state));
    }
    
    destroy_state_machine(sm);
    return 0;
}