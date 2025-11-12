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
    size_t transition_count;
    int *data_stack;
    size_t stack_size;
    size_t stack_capacity;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(StateTransition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->stack_capacity = 10;
    sm->stack_size = 0;
    sm->data_stack = malloc(sm->stack_capacity * sizeof(int));
    if (!sm->data_stack) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->data_stack);
        free(sm);
    }
}

int add_transition(StateMachine *sm, SystemState from, SystemState to, int trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

int push_data(StateMachine *sm, int value) {
    if (!sm || sm->stack_size >= sm->stack_capacity) {
        size_t new_capacity = sm->stack_capacity * 2;
        int *new_stack = realloc(sm->data_stack, new_capacity * sizeof(int));
        if (!new_stack) return 0;
        sm->data_stack = new_stack;
        sm->stack_capacity = new_capacity;
    }
    
    sm->data_stack[sm->stack_size] = value;
    sm->stack_size++;
    return 1;
}

int pop_data(StateMachine *sm, int *value) {
    if (!sm || !value || sm->stack_size == 0) return 0;
    sm->stack_size--;
    *value = sm->data_stack[sm->stack_size];
    return 1;
}

SystemState process_trigger(StateMachine *sm, int trigger) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].to_state;
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
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1) ||
        !add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2) ||
        !add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 3) ||
        !add_transition(sm, STATE_ACTIVE, STATE_DONE, 4) ||
        !add_transition(sm, STATE_PAUSED, STATE_DONE, 5) ||
        !add_transition(sm, STATE_ACTIVE, STATE_ERROR, 0) ||
        !add_transition(sm, STATE_PAUSED, STATE_ERROR, 0)) {
        fprintf(stderr, "Failed to add transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("State Machine Demo\n");
    printf("Available triggers: 1=Start, 2=Pause, 3=Resume, 4=Complete, 5=Finish, 0=Error\n");
    
    char input[BUFFER_SIZE];
    int trigger;
    
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        printf("\nCurrent state: ");
        print_state(sm->current_state);
        printf("\nEnter trigger: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        if (sscanf(input,