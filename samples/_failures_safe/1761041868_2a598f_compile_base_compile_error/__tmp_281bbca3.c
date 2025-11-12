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
} SystemState;

typedef struct {
    SystemState from_state;
    SystemState to_state;
    char trigger[32];
} StateTransition;

typedef struct {
    SystemState current_state;
    StateTransition *transitions;
    size_t transition_count;
    size_t transition_capacity;
    char *buffer;
    size_t buffer_size;
} StateMachine;

int state_machine_init(StateMachine *sm) {
    if (!sm) return 0;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->transition_capacity = 4;
    
    sm->transitions = malloc(sizeof(StateTransition) * sm->transition_capacity);
    if (!sm->transitions) return 0;
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        return 0;
    }
    sm->buffer_size = BUFFER_SIZE;
    
    return 1;
}

void state_machine_cleanup(StateMachine *sm) {
    if (!sm) return;
    
    if (sm->transitions) {
        free(sm->transitions);
    }
    if (sm->buffer) {
        free(sm->buffer);
    }
}

int add_transition(StateMachine *sm, SystemState from, SystemState to, const char *trigger) {
    if (!sm || !trigger) return 0;
    
    if (sm->transition_count >= sm->transition_capacity) {
        size_t new_capacity = sm->transition_capacity * 2;
        if (new_capacity > MAX_TRANSITIONS) new_capacity = MAX_TRANSITIONS;
        
        StateTransition *new_transitions = realloc(sm->transitions, 
                                                  sizeof(StateTransition) * new_capacity);
        if (!new_transitions) return 0;
        
        sm->transitions = new_transitions;
        sm->transition_capacity = new_capacity;
    }
    
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    StateTransition *t = &sm->transitions[sm->transition_count];
    t->from_state = from;
    t->to_state = to;
    
    size_t len = strlen(trigger);
    if (len >= sizeof(t->trigger)) len = sizeof(t->trigger) - 1;
    strncpy(t->trigger, trigger, len);
    t->trigger[len] = '\0';
    
    sm->transition_count++;
    return 1;
}

SystemState process_transition(StateMachine *sm, const char *input) {
    if (!sm || !input || sm->current_state == STATE_ERROR) return sm->current_state;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        StateTransition *t = &sm->transitions[i];
        if (t->from_state == sm->current_state && strcmp(t->trigger, input) == 0) {
            sm->current_state = t->to_state;
            break;
        }
    }
    
    return sm->current_state;
}

void print_state(SystemState state) {
    switch (state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_ACTIVE: printf("ACTIVE"); break;
        case STATE_PAUSED: printf("PAUSED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        case STATE_DONE: printf("DONE"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    StateMachine sm;
    if (!state_machine_init(&sm)) {
        fprintf(stderr, "Failed to initialize state machine\n");
        return 1;
    }
    
    int transitions_added = 0;
    transitions_added += add_transition(&sm, STATE_IDLE, STATE_ACTIVE, "start");
    transitions_added += add_transition(&sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    transitions_added += add_transition(&sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    transitions_added += add_transition(&sm, STATE_ACTIVE, STATE_DONE, "complete");
    transitions_added += add_transition(&sm, STATE_ACTIVE, STATE_ERROR, "error");
    transitions_added += add_transition(&sm, STATE_PAUSED, STATE_ERROR, "error");
    
    if (transitions_added != 6) {
        fprintf(stderr, "Failed to add all transitions\n");
        state_machine_cleanup(&sm);
        return 1;
    }
    
    printf("State Machine Simulation\n");
    printf("Available commands: start, pause, resume, complete, error\n");
    printf("Type 'quit' to exit\n\n");
    
    char input[64];
    
    while (1) {
        printf("Current state: ");
        print_state(sm.current_state);
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;