//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    State from;
    State to;
    char trigger;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    char *buffer;
    size_t buffer_size;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
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
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to;
            break;
        }
    }
    
    return sm->current_state;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 'f');
    add_transition(sm, STATE_ACTIVE, STATE_ERROR, 'e');
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(sm, STATE_PAUSED, STATE_ERROR, 'e');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    add_transition(sm, STATE_DONE, STATE_IDLE, 'r');
}

const char* state_name(State state) {
    switch(state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
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
    
    initialize_transitions(sm);
    
    printf("State Machine Controller\n");
    printf("Commands: s=start, p=pause, r=resume/reset, f=finish, e=error, q=quit\n");
    
    char input;
    int running = 1;
    
    while (running) {
        printf("Current state: %s\n", state_name(sm->current_state));
        printf("Enter command: ");
        
        if (scanf(" %c", &input) != 1) {
            fprintf(stderr, "Invalid input\n");
            break;
        }
        
        if (input == 'q') {
            running = 0;
            continue;
        }
        
        State new_state = process_input(sm, input);
        printf("Transitioned to: %s\n", state_name(new_state));
        
        if (sm->current_state == STATE_ERROR) {
            printf("Error state detected. Use 'r' to reset.\n");
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}