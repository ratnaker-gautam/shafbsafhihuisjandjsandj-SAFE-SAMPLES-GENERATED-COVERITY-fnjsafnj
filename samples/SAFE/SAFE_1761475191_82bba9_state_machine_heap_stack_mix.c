//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    STATE_E,
    NUM_STATES
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int num_transitions;
    char *name;
} StateMachine;

StateMachine* create_state_machine(const char *name) {
    if (name == NULL || strlen(name) == 0) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->name = malloc(strlen(name) + 1);
    if (sm->name == NULL) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm->name);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->num_transitions = 0;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->transitions);
        free(sm->name);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm == NULL || sm->num_transitions >= MAX_TRANSITIONS) {
        return 0;
    }
    
    sm->transitions[sm->num_transitions].current = from;
    sm->transitions[sm->num_transitions].input = input;
    sm->transitions[sm->num_transitions].next = to;
    sm->num_transitions++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (sm == NULL) {
        return NUM_STATES;
    }
    
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_A: return "STATE_A";
        case STATE_B: return "STATE_B";
        case STATE_C: return "STATE_C";
        case STATE_D: return "STATE_D";
        case STATE_E: return "STATE_E";
        default: return "INVALID_STATE";
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    StateMachine *sm = create_state_machine("DemoStateMachine");
    
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_C, '4', STATE_D);
    add_transition(sm, STATE_D, '5', STATE_E);
    add_transition(sm, STATE_E, '0', STATE_A);
    
    printf("State Machine: %s\n", sm->name);
    printf("Current state: %s\n", state_to_string(sm->current_state));
    printf("Enter input characters (q to quit):\n");
    
    while (1) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        char input_char = input_buffer[0];
        if (input_char == 'q' || input_char == 'Q') {
            break;
        }
        
        State new_state = process_input(sm, input_char);
        printf("Input: '%c', New state: %s\n", input_char, state_to_string(new_state));
    }
    
    destroy_state_machine(sm);
    return 0;
}