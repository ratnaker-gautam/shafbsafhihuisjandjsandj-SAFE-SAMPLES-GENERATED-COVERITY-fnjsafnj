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

StateMachine* create_state_machine(int max_states, int max_transitions) {
    if (max_states <= 0 || max_transitions <= 0 || max_states > MAX_STATES || max_transitions > MAX_TRANSITIONS) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(sizeof(State) * max_states);
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->transitions = malloc(sizeof(Transition) * max_transitions);
    if (!sm->transitions) {
        free(sm->states);
        free(sm);
        return NULL;
    }
    
    sm->state_count = 0;
    sm->transition_count = 0;
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

int add_state(StateMachine *sm, State state) {
    if (!sm || sm->state_count >= MAX_STATES) return 0;
    sm->states[sm->state_count++] = state;
    return 1;
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, char trigger) {
    if (!sm) return STATE_ERROR;
    
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
    StateMachine *sm = create_state_machine(5, 8);
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, STATE_IDLE);
    add_state(sm, STATE_ACTIVE);
    add_state(sm, STATE_PAUSED);
    add_state(sm, STATE_ERROR);
    add_state(sm, STATE_DONE);
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 'f');
    add_transition(sm, STATE_PAUSED, STATE_IDLE, 'c');
    add_transition(sm, STATE_ACTIVE, STATE_ERROR, 'e');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    add_transition(sm, STATE_DONE, STATE_IDLE, 'r');
    
    printf("State Machine Controller\n");
    printf("Commands: s=start, p=pause, r=resume/reset, f=finish, e=error, c=cancel, q=quit\n");
    
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter command: ");
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break;
        }
        
        if (buffer[0] == 'q' && (buffer[1] == '\n' || buffer[1] == '\0')) {
            break;
        }
        
        if (buffer[0] != '\n' && buffer[1] == '\n') {
            process_trigger(sm, buffer[0]);
        } else {
            printf("Invalid input. Use single character commands.\n");
        }
        
        if (sm->current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}