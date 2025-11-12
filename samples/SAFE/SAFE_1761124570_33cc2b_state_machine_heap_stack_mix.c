//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    STATE_PAUSED,
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

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 5;
    sm->states = malloc(sizeof(State) * sm->state_count);
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->states[0] = STATE_IDLE;
    sm->states[1] = STATE_ACTIVE;
    sm->states[2] = STATE_PAUSED;
    sm->states[3] = STATE_ERROR;
    sm->states[4] = STATE_DONE;
    
    sm->transition_count = 6;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm->states);
        free(sm);
        return NULL;
    }
    
    sm->transitions[0] = (Transition){STATE_IDLE, STATE_ACTIVE, 's'};
    sm->transitions[1] = (Transition){STATE_ACTIVE, STATE_PAUSED, 'p'};
    sm->transitions[2] = (Transition){STATE_ACTIVE, STATE_DONE, 'f'};
    sm->transitions[3] = (Transition){STATE_ACTIVE, STATE_ERROR, 'e'};
    sm->transitions[4] = (Transition){STATE_PAUSED, STATE_ACTIVE, 'r'};
    sm->transitions[5] = (Transition){STATE_ERROR, STATE_IDLE, 'r'};
    
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

State process_transition(StateMachine *sm, char trigger) {
    if (!sm) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return sm->current_state;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    printf("State Machine Controller\n");
    printf("Available triggers: s(start), p(pause), r(resume), f(finish), e(error), q(quit)\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter trigger: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (input[0] == 'q') {
            break;
        }
        
        if (strlen(input) < 1) {
            continue;
        }
        
        State previous = sm->current_state;
        process_transition(sm, input[0]);
        
        if (previous != sm->current_state) {
            printf("Transition: %s -> %s\n", state_to_string(previous), state_to_string(sm->current_state));
        } else {
            printf("No valid transition for trigger '%c'\n", input[0]);
        }
        
        if (sm->current_state == STATE_DONE) {
            printf("Process completed successfully\n");
            break;
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}