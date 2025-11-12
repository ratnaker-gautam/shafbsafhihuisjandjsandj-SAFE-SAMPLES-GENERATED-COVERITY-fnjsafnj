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
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    if (sm->transitions) free(sm->transitions);
    free(sm);
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

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    int stack_transitions[MAX_TRANSITIONS];
    int stack_count = 0;
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1);
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2);
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 3);
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 4);
    add_transition(sm, STATE_PAUSED, STATE_ERROR, 5);
    add_transition(sm, STATE_ERROR, STATE_IDLE, 6);
    
    printf("State Machine Simulation\n");
    printf("Available triggers: 1=Start, 2=Pause, 3=Complete, 4=Resume, 5=Error, 6=Reset, 0=Exit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        print_state(sm->current_state);
        printf("\nEnter trigger (0-6): ");
        
        int trigger;
        if (scanf("%d", &trigger) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (trigger == 0) break;
        
        if (trigger < 1 || trigger > 6) {
            printf("Invalid trigger\n");
            continue;
        }
        
        SystemState old_state = sm->current_state;
        SystemState new_state = process_trigger(sm, trigger);
        
        if (stack_count < MAX_TRANSITIONS) {
            stack_transitions[stack_count++] = trigger;
        }
        
        printf("Transition: ");
        print_state(old_state);
        printf(" -> ");
        print_state(new_state);
        printf(" (trigger: %d)\n", trigger);
        
        if (new_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    printf("\nTransition history (stack): ");
    for (int i = 0; i < stack_count; i++) {
        printf("%d", stack_transitions[i]);
        if (i < stack_count - 1) printf(" -> ");
    }
    printf("\n");
    
    destroy_state_machine(sm);
    return 0;
}