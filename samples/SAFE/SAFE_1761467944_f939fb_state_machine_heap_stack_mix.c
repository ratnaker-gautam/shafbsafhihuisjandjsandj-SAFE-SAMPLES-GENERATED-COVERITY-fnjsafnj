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
    int max_transitions;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->max_transitions = MAX_TRANSITIONS;
    sm->transition_count = 0;
    
    sm->transitions = malloc(sizeof(Transition) * sm->max_transitions);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || !sm->transitions) return 0;
    if (sm->transition_count >= sm->max_transitions) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm || !sm->transitions) return sm ? sm->current_state : STATE_A;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        if (sm->transitions) {
            free(sm->transitions);
        }
        free(sm);
    }
}

int main() {
    char input_buffer[MAX_INPUT_SIZE];
    StateMachine *sm = create_state_machine();
    
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_B, '4', STATE_E);
    add_transition(sm, STATE_C, '5', STATE_D);
    add_transition(sm, STATE_C, '6', STATE_E);
    add_transition(sm, STATE_D, '7', STATE_A);
    add_transition(sm, STATE_E, '8', STATE_A);
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: 1-8\n");
    printf("Enter 'q' to quit\n");
    printf("Current state: A\n");
    
    while (1) {
        printf("Enter input: ");
        
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (strlen(input_buffer) > 0 && input_buffer[0] == 'q') {
            break;
        }
        
        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }
        
        char input_char = input_buffer[0];
        
        if (input_char < '1' || input_char > '8') {
            printf("Invalid input. Please enter 1-8 or 'q' to quit.\n");
            continue;
        }
        
        State new_state = process_input(sm, input_char);
        
        printf("Transitioned to state: ");
        switch (new_state) {
            case STATE_A: printf("A\n"); break;
            case STATE_B: printf("B\n"); break;
            case STATE_C: printf("C\n"); break;
            case STATE_D: printf("D\n"); break;
            case STATE_E: printf("E\n"); break;
            default: printf("Unknown\n"); break;
        }
    }
    
    destroy_state_machine(sm);
    printf("State machine simulation ended.\n");
    
    return 0;
}