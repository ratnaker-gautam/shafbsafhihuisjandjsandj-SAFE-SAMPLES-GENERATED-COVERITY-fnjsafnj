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
} SystemState;

typedef struct {
    SystemState from;
    SystemState to;
    int trigger;
} Transition;

typedef struct {
    SystemState current_state;
    Transition *transitions;
    int transition_count;
    int max_transitions;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->max_transitions = MAX_TRANSITIONS;
    sm->transition_count = 0;
    
    sm->transitions = malloc(sizeof(Transition) * sm->max_transitions);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine *sm, SystemState from, SystemState to, int trigger) {
    if (!sm || sm->transition_count >= sm->max_transitions) {
        return 0;
    }
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->from = from;
    t->to = to;
    t->trigger = trigger;
    sm->transition_count++;
    
    return 1;
}

SystemState process_trigger(StateMachine *sm, int trigger) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->from == sm->current_state && t->trigger == trigger) {
            sm->current_state = t->to;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

const char* state_to_string(SystemState state) {
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
    
    int transitions_added = 0;
    transitions_added += add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1);
    transitions_added += add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2);
    transitions_added += add_transition(sm, STATE_ACTIVE, STATE_DONE, 3);
    transitions_added += add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 4);
    transitions_added += add_transition(sm, STATE_PAUSED, STATE_ERROR, 5);
    transitions_added += add_transition(sm, STATE_ERROR, STATE_IDLE, 6);
    
    if (transitions_added != 6) {
        fprintf(stderr, "Failed to add all transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("State Machine initialized. Current state: %s\n", state_to_string(sm->current_state));
    printf("Available triggers: 1=Start, 2=Pause, 3=Complete, 4=Resume, 5=Error, 6=Reset, 0=Exit\n");
    
    char input[BUFFER_SIZE];
    int trigger;
    
    while (1) {
        printf("\nEnter trigger (0-6): ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        if (sscanf(input, "%d", &trigger) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (trigger == 0) {
            printf("Exiting...\n");
            break;
        }
        
        if (trigger < 1 || trigger > 6) {
            printf("Invalid trigger. Must be between 1-6.\n");
            continue;
        }
        
        SystemState old_state = sm->current_state;
        SystemState new_state = process_trigger(sm, trigger);
        
        printf("State transition: %s -> %s (trigger: %d)\n", 
               state_to_string(old_state), state_to_string(new_state), trigger);
    }
    
    destroy_state_machine(sm);
    return 0;
}