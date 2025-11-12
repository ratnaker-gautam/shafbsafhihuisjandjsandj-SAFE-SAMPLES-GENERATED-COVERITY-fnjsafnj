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
    State current_state;
    Transition *transitions;
    size_t transition_count;
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
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, char trigger) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return STATE_ERROR;
}

int process_input(StateMachine *sm, const char *input) {
    if (!sm || !input) return 0;
    
    size_t len = strlen(input);
    if (len >= sm->buffer_size) return 0;
    
    strncpy(sm->buffer, input, sm->buffer_size - 1);
    sm->buffer[sm->buffer_size - 1] = '\0';
    
    for (size_t i = 0; i < len; i++) {
        State result = process_trigger(sm, input[i]);
        if (result == STATE_ERROR) {
            return 0;
        }
    }
    return 1;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(sm, STATE_ACTIVE, STATE_PROCESSING, 'p');
    add_transition(sm, STATE_PROCESSING, STATE_DONE, 'd');
    add_transition(sm, STATE_PROCESSING, STATE_ERROR, 'e');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    add_transition(sm, STATE_DONE, STATE_IDLE, 'r');
}

const char* state_to_string(State state) {
    switch(state) {
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
    
    initialize_transitions(sm);
    
    char input[BUFFER_SIZE];
    printf("Enter state sequence (s=start, p=process, d=done, e=error, r=reset): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    if (process_input(sm, input)) {
        printf("Final state: %s\n", state_to_string(sm->current_state));
    } else {
        printf("Invalid transition sequence. Current state: %s\n", 
               state_to_string(sm->current_state));
    }
    
    destroy_state_machine(sm);
    return 0;
}