//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    size_t transition_count;
    char *input_buffer;
    size_t input_len;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->input_len = 0;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->input_buffer = malloc(MAX_INPUT_LEN);
    if (!sm->input_buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->input_buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return STATE_A;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_B, '4', STATE_E);
    add_transition(sm, STATE_C, '5', STATE_D);
    add_transition(sm, STATE_C, '6', STATE_E);
    add_transition(sm, STATE_D, '7', STATE_A);
    add_transition(sm, STATE_E, '8', STATE_A);
}

const char* state_to_string(State s) {
    switch(s) {
        case STATE_A: return "STATE_A";
        case STATE_B: return "STATE_B";
        case STATE_C: return "STATE_C";
        case STATE_D: return "STATE_D";
        case STATE_E: return "STATE_E";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    printf("State Machine initialized. Current state: %s\n", state_to_string(sm->current_state));
    printf("Valid inputs: 1-8. Enter 'q' to quit.\n");
    
    while (1) {
        printf("Enter input: ");
        
        if (fgets(sm->input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
            break;
        }
        
        if (sm->input_buffer[0] == 'q' && (sm->input_buffer[1] == '\n' || sm->input_buffer[1] == '\0')) {
            break;
        }
        
        if (sm->input_buffer[0] >= '1' && sm->input_buffer[0] <= '8' && 
            (sm->input_buffer[1] == '\n' || sm->input_buffer[1] == '\0')) {
            State new_state = process_input(sm, sm->input_buffer[0]);
            printf("New state: %s\n", state_to_string(new_state));
        } else {
            printf("Invalid input. Please enter 1-8 or 'q' to quit.\n");
        }
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    destroy_state_machine(sm);
    return 0;
}