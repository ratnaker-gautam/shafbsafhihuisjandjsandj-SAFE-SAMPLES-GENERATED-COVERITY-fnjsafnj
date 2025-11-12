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
    STATE_COMPLETE,
    STATE_ERROR
} SystemState;

typedef struct {
    SystemState current;
    SystemState next;
    int trigger;
} Transition;

typedef struct {
    SystemState state;
    int data;
    char* message;
} StateContext;

typedef struct {
    Transition* transitions;
    int transition_count;
    StateContext* context_stack;
    int stack_top;
    int stack_size;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->transition_count = 0;
    sm->stack_size = 5;
    sm->stack_top = -1;
    
    sm->context_stack = malloc(sm->stack_size * sizeof(StateContext));
    if (!sm->context_stack) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (!sm) return;
    
    for (int i = 0; i <= sm->stack_top; i++) {
        if (sm->context_stack[i].message) {
            free(sm->context_stack[i].message);
        }
    }
    
    free(sm->context_stack);
    free(sm->transitions);
    free(sm);
}

int add_transition(StateMachine* sm, SystemState from, SystemState to, int trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

int push_state(StateMachine* sm, StateContext context) {
    if (!sm || sm->stack_top >= sm->stack_size - 1) return 0;
    
    sm->stack_top++;
    sm->context_stack[sm->stack_top] = context;
    return 1;
}

StateContext pop_state(StateMachine* sm) {
    StateContext empty = {STATE_ERROR, 0, NULL};
    if (!sm || sm->stack_top < 0) return empty;
    
    return sm->context_stack[sm->stack_top--];
}

SystemState process_trigger(StateMachine* sm, SystemState current, int trigger) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == current && sm->transitions[i].trigger == trigger) {
            return sm->transitions[i].next;
        }
    }
    return current;
}

const char* state_to_string(SystemState state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1);
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2);
    add_transition(sm, STATE_ACTIVE, STATE_COMPLETE, 3);
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 4);
    add_transition(sm, STATE_PAUSED, STATE_ERROR, 5);
    add_transition(sm, STATE_ERROR, STATE_IDLE, 6);
    
    StateContext initial_context = {STATE_IDLE, 100, NULL};
    char* msg = malloc(32);
    if (msg) {
        strncpy(msg, "Initial state", 31);
        msg[31] = '\0';
        initial_context.message = msg;
    }
    
    if (!push_state(sm, initial_context)) {
        printf("Failed to push initial state\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("State Machine Simulation\n");
    printf("Triggers: 1=Start, 2=Pause, 3=Complete, 4=Resume, 5=Error, 6=Reset, 0=Exit\n");
    
    int running = 1;
    char input[BUFFER_SIZE];
    
    while (running) {
        StateContext current = sm->context_stack[sm->stack_top];
        printf("\nCurrent state: %s (data: %d", state_to_string(current.state), current.data);
        if (current.message) {
            printf(", message: %s", current.message);
        }
        printf(")\n");
        
        printf("Enter trigger: ");
        if (!fgets(input, sizeof(input