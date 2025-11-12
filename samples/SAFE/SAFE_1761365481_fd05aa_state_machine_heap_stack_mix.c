//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    State next;
    char trigger;
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
    
    sm->states = malloc(MAX_STATES * sizeof(State));
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->states || !sm->transitions) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states[0] = STATE_IDLE;
    sm->states[1] = STATE_ACTIVE;
    sm->states[2] = STATE_PROCESSING;
    sm->states[3] = STATE_ERROR;
    sm->states[4] = STATE_DONE;
    
    sm->transition_count = 6;
    sm->transitions[0] = (Transition){STATE_IDLE, STATE_ACTIVE, 's'};
    sm->transitions[1] = (Transition){STATE_ACTIVE, STATE_PROCESSING, 'p'};
    sm->transitions[2] = (Transition){STATE_PROCESSING, STATE_DONE, 'c'};
    sm->transitions[3] = (Transition){STATE_PROCESSING, STATE_ERROR, 'e'};
    sm->transitions[4] = (Transition){STATE_ERROR, STATE_IDLE, 'r'};
    sm->transitions[5] = (Transition){STATE_DONE, STATE_IDLE, 'r'};
    
    sm->current_state = STATE_IDLE;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

State process_transition(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == input) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    char input;
    
    printf("State Machine Controller\n");
    printf("Available commands: s(start), p(process), c(complete), e(error), r(reset), q(quit)\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter command: ");
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        if (sscanf(buffer, " %c", &input) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (input == 'q') {
            break;
        }
        
        State new_state = process_transition(sm, input);
        if (new_state != sm->current_state) {
            printf("Transition: %s -> %s\n", 
                   state_to_string(sm->current_state), 
                   state_to_string(new_state));
            sm->current_state = new_state;
        } else {
            printf("Invalid transition from current state\n");
        }
        
        if (sm->current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}