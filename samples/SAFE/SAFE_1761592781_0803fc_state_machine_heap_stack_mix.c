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
    SystemState from_state;
    SystemState to_state;
    int trigger;
} StateTransition;

typedef struct {
    SystemState current_state;
    StateTransition *transitions;
    int transition_count;
    int max_transitions;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->max_transitions = MAX_TRANSITIONS;
    sm->transition_count = 0;
    
    sm->transitions = malloc(sizeof(StateTransition) * sm->max_transitions);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

int add_transition(StateMachine *sm, SystemState from, SystemState to, int trigger) {
    if (!sm || !sm->transitions) return 0;
    if (sm->transition_count >= sm->max_transitions) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    
    return 1;
}

SystemState process_trigger(StateMachine *sm, int trigger) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].to_state;
            return sm->current_state;
        }
    }
    
    return STATE_ERROR;
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    if (sm->transitions) free(sm->transitions);
    free(sm);
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
        printf("Failed to create state machine\n");
        return 1;
    }
    
    int stack_transitions[MAX_TRANSITIONS];
    int stack_count = 0;
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1)) return 1;
    if (!add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2)) return 1;
    if (!add_transition(sm, STATE_ACTIVE, STATE_DONE, 3)) return 1;
    if (!add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 4)) return 1;
    if (!add_transition(sm, STATE_PAUSED, STATE_ERROR, 5)) return 1;
    
    printf("State Machine Simulation\n");
    printf("Available triggers: 1=Start, 2=Pause, 3=Complete, 4=Resume, 5=Error, 0=Exit\n");
    
    while (1) {
        printf("\nCurrent state: %s\n", state_to_string(sm->current_state));
        printf("Enter trigger (0-5): ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 0) break;
        
        if (input < 0 || input > 5) {
            printf("Invalid trigger\n");
            continue;
        }
        
        if (stack_count < MAX_TRANSITIONS) {
            stack_transitions[stack_count++] = input;
        }
        
        SystemState new_state = process_trigger(sm, input);
        printf("Trigger %d: %s -> %s\n", input, 
               state_to_string(sm->current_state), state_to_string(new_state));
        
        if (new_state == STATE_ERROR) {
            printf("Invalid transition from current state\n");
            sm->current_state = STATE_ERROR;
        }
        
        if (new_state == STATE_DONE || new_state == STATE_ERROR) {
            printf("Final state reached: %s\n", state_to_string(new_state));
            break;
        }
    }
    
    printf("\nTransition history: ");
    for (int i = 0; i < stack_count; i++) {
        printf("%d ", stack_transitions[i]);
    }
    printf("\n");
    
    destroy_state_machine(sm);
    return 0;
}