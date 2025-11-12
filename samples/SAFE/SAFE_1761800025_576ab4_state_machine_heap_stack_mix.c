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
    STATE_E,
    NUM_STATES
} State;

typedef struct {
    State from;
    State to;
    char trigger;
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
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->input_buffer = malloc(MAX_INPUT_LEN + 1);
    if (!sm->input_buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->input_len = 0;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->input_buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].from = from;
    sm->transitions[sm->transition_count].to = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm ? sm->current_state : STATE_A;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to;
            break;
        }
    }
    
    return sm->current_state;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, STATE_B, '1');
    add_transition(sm, STATE_A, STATE_C, '2');
    add_transition(sm, STATE_B, STATE_C, '2');
    add_transition(sm, STATE_B, STATE_D, '3');
    add_transition(sm, STATE_C, STATE_D, '3');
    add_transition(sm, STATE_C, STATE_E, '4');
    add_transition(sm, STATE_D, STATE_E, '4');
    add_transition(sm, STATE_E, STATE_A, '0');
}

const char* state_name(State s) {
    static const char* names[] = {"A", "B", "C", "D", "E"};
    if (s < NUM_STATES) return names[s];
    return "UNKNOWN";
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: 0,1,2,3,4\n");
    printf("Enter 'q' to quit\n");
    printf("Current state: %s\n", state_name(sm->current_state));
    
    while (1) {
        printf("Enter input: ");
        fflush(stdout);
        
        char input[10];
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }
        
        if (strlen(input) > 2 || (input[0] < '0' || input[0] > '4')) {
            printf("Invalid input. Please enter 0-4 or 'q' to quit.\n");
            continue;
        }
        
        State new_state = process_input(sm, input[0]);
        printf("Transitioned to state: %s\n", state_name(new_state));
    }
    
    destroy_state_machine(sm);
    return 0;
}