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
    
    sm->data_stack[sm->stack_size++] = value;
    return 1;
}

int pop_data(StateMachine *sm, int *value) {
    if (!sm || !value || sm->stack_size == 0) return 0;
    *value = sm->data_stack[--sm->stack_size];
    return 1;
}

int process_trigger(StateMachine *sm, int trigger) {
    if (!sm) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void print_state(SystemState state) {
    const char *state_names[] = {
        "IDLE", "ACTIVE", "PAUSED", "ERROR", "DONE"
    };
    printf("Current state: %s\n", state_names[state]);
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1);
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2);
    add_transition(sm, STATE_ACTIVE, STATE_ERROR, 3);
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 4);
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 5);
    add_transition(sm, STATE_PAUSED, STATE_ERROR, 6);
    add_transition(sm, STATE_ERROR, STATE_IDLE, 7);
    
    char input[BUFFER_SIZE];
    int trigger;
    
    printf("State Machine Controller\n");
    printf("Triggers: 1=Start, 2=Pause, 3=Error, 4=Complete, 5=Resume, 6=Error, 7=Reset\n");
    printf("Enter 'q' to quit\n\n");
    
    while (1) {
        print_state(sm->current_state);
        printf("Enter trigger (1-7) or 'q' to quit: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
        
        if (sscanf(input, "%d", &trigger) != 1) {
            printf("Invalid input. Please enter a number 1-7 or 'q'.\n");
            continue;
        }
        
        if (trigger < 1 || trigger > 7) {
            printf("