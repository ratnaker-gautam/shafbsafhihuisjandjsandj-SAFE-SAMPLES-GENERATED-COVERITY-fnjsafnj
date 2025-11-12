//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    int transition_count;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void print_state(State state) {
    const char *state_names[] = {"A", "B", "C", "D", "E"};
    if (state >= 0 && state < NUM_STATES) {
        printf("Current state: %s\n", state_names[state]);
    }
}

void cleanup_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_C, '1', STATE_D);
    add_transition(sm, STATE_D, '0', STATE_E);
    add_transition(sm, STATE_D, '1', STATE_E);
    add_transition(sm, STATE_E, '0', STATE_E);
    add_transition(sm, STATE_E, '1', STATE_E);
    
    char input_buffer[MAX_INPUT_SIZE];
    
    printf("State Machine Simulation (enter 'q' to quit)\n");
    printf("Valid inputs: '0' or '1'\n");
    
    while (1) {
        print_state(sm->current_state);
        printf("Enter input: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) > 0 && input_buffer[0] == 'q') {
            break;
        }
        
        if (strlen(input_buffer) == 2 && (input_buffer[0] == '0' || input_buffer[0] == '1')) {
            if (process_input(sm, input_buffer[0])) {
                printf("Transition successful\n");
            } else {
                printf("No valid transition for input '%c' in current state\n", input_buffer[0]);
            }
        } else {
            printf("Invalid input. Please enter '0', '1', or 'q' to quit.\n");
        }
    }
    
    cleanup_state_machine(sm);
    return 0;
}