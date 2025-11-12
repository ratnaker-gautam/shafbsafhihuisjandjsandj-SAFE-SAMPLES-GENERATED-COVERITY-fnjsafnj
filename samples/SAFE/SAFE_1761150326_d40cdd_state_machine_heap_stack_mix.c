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
    State *states;
    Transition *transitions;
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 5;
    sm->states = malloc(sizeof(State) * sm->state_count);
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    
    sm->transition_count = 8;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm->states);
        free(sm);
        return NULL;
    }
    
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_A};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_A};
    sm->transitions[5] = (Transition){STATE_C, '1', STATE_E};
    sm->transitions[6] = (Transition){STATE_D, '0', STATE_E};
    sm->transitions[7] = (Transition){STATE_D, '1', STATE_B};
    
    sm->current_state = STATE_A;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
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

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation (Enter 'q' to quit)\n");
    printf("Valid inputs: 0, 1\n");
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    while (1) {
        printf("\nCurrent state: %s\n", state_to_string(sm->current_state));
        printf("Enter input (0 or 1): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0 || input_buffer[0] == 'q') {
            break;
        }
        
        char input_char = input_buffer[0];
        if (input_char != '0' && input_char != '1') {
            printf("Invalid input. Please enter 0 or 1.\n");
            continue;
        }
        
        State new_state = process_input(sm, input_char);
        printf("Transition: %s -> %s with input '%c'\n", 
               state_to_string(sm->current_state), state_to_string(new_state), input_char);
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    destroy_state_machine(sm);
    return 0;
}