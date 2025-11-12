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
    int max_transitions;
} StateMachine;

StateMachine* create_state_machine(int max_trans) {
    if (max_trans <= 0 || max_trans > MAX_TRANSITIONS) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) {
        return NULL;
    }
    
    sm->transitions = malloc(sizeof(Transition) * max_trans);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->max_transitions = max_trans;
    
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= sm->max_transitions) {
        return 0;
    }
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->input = input;
    t->next = to;
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) {
        return STATE_A;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && t->input == input) {
            sm->current_state = t->next;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

int main() {
    char input_buffer[MAX_INPUT_LEN];
    StateMachine *sm = create_state_machine(8);
    
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_B, '2', STATE_C);
    add_transition(sm, STATE_C, '3', STATE_D);
    add_transition(sm, STATE_D, '4', STATE_E);
    add_transition(sm, STATE_E, '5', STATE_A);
    add_transition(sm, STATE_B, '0', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_D, '0', STATE_A);
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: 0-5, 'q' to quit\n");
    printf("Current state: A\n");
    
    while (1) {
        printf("Enter input: ");
        
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        char input_char = input_buffer[0];
        
        if (input_char == 'q' || input_char == 'Q') {
            break;
        }
        
        if (input_char < '0' || input_char > '5') {
            printf("Invalid input. Please enter 0-5 or 'q' to quit.\n");
            continue;
        }
        
        State new_state = process_input(sm, input_char);
        
        printf("Input: %c -> New state: ", input_char);
        switch (new_state) {
            case STATE_A: printf("A\n"); break;
            case STATE_B: printf("B\n"); break;
            case STATE_C: printf("C\n"); break;
            case STATE_D: printf("D\n"); break;
            case STATE_E: printf("E\n"); break;
        }
    }
    
    destroy_state_machine(sm);
    printf("State machine simulation ended.\n");
    
    return 0;
}