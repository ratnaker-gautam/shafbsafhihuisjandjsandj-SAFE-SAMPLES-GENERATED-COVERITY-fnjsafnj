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
    int capacity;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->capacity = 5;
    sm->transition_count = 0;
    sm->transitions = malloc(sm->capacity * sizeof(Transition));
    
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || !sm->transitions) return 0;
    
    if (sm->transition_count >= sm->capacity) {
        int new_capacity = sm->capacity * 2;
        if (new_capacity > MAX_TRANSITIONS) new_capacity = MAX_TRANSITIONS;
        
        Transition *new_trans = realloc(sm->transitions, new_capacity * sizeof(Transition));
        if (!new_trans) return 0;
        
        sm->transitions = new_trans;
        sm->capacity = new_capacity;
    }
    
    if (sm->transition_count < sm->capacity) {
        sm->transitions[sm->transition_count].current = from;
        sm->transitions[sm->transition_count].input = input;
        sm->transitions[sm->transition_count].next = to;
        sm->transition_count++;
        return 1;
    }
    
    return 0;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return STATE_A;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
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

const char* state_to_string(State s) {
    switch (s) {
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
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation (Enter 'q' to quit)\n");
    printf("Valid inputs: 0, 1\n");
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    while (1) {
        printf("Enter input: ");
        
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer) - 1] == '\n') {
            input_buffer[strlen(input_buffer) - 1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        if (strcmp(input_buffer, "q") == 0) {
            break;
        }
        
        if (strlen(input_buffer) != 1 || (input_buffer[0] != '0' && input_buffer[0] != '1')) {
            printf("Invalid input. Please enter 0, 1, or q to quit.\n");
            continue;
        }
        
        State new_state = process_input(sm, input_buffer[0]);
        printf("Input: %c -> New State: %s\n", input_buffer[0], state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    destroy_state_machine(sm);
    
    return 0;
}