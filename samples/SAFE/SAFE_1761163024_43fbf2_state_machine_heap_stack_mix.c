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
    
    return STATE_ERROR;
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
    
    int stack_transitions[MAX_TRANSITIONS][3] = {
        {STATE_IDLE, STATE_ACTIVE, 1},
        {STATE_ACTIVE, STATE_PAUSED, 2},
        {STATE_PAUSED, STATE_ACTIVE, 3},
        {STATE_ACTIVE, STATE_DONE, 4},
        {STATE_PAUSED, STATE_IDLE, 5},
        {STATE_IDLE, STATE_ERROR, 6},
        {STATE_ERROR, STATE_IDLE, 7}
    };
    
    for (int i = 0; i < 7; i++) {
        if (!add_transition(sm, stack_transitions[i][0], stack_transitions[i][1], stack_transitions[i][2])) {
            fprintf(stderr, "Failed to add transition\n");
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    printf("State Machine initialized. Current state: %s\n", state_to_string(sm->current_state));
    printf("Available triggers: 1=Start, 2=Pause, 3=Resume, 4=Complete, 5=Cancel, 6=Error, 7=Reset\n");
    
    char input[BUFFER_SIZE];
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        printf("Enter trigger (1-7) or 'q' to quit: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }
        
        char *endptr;
        long trigger = strtol(input, &endptr, 10);
        if (endptr == input || trigger < 1 || trigger > 7) {
            printf("Invalid input. Please enter 1-7 or 'q'.\n");
            continue;
        }
        
        SystemState new_state = process_trigger(sm, (int)trigger);
        printf("Trigger %ld: %s -> %s\n", trigger, state_to_string(sm->current_state), state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    destroy_state_machine(sm);
    
    return 0;
}