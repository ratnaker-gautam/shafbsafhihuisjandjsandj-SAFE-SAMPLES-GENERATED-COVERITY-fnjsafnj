//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    NUM_STATES
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State *stack;
    int stack_top;
    int stack_size;
} StateStack;

typedef struct {
    State current_state;
    Transition *transitions;
    int num_transitions;
    StateStack *stack;
} StateMachine;

StateStack* create_stack(int size) {
    if (size <= 0) return NULL;
    StateStack *stack = malloc(sizeof(StateStack));
    if (!stack) return NULL;
    stack->stack = malloc(sizeof(State) * size);
    if (!stack->stack) {
        free(stack);
        return NULL;
    }
    stack->stack_top = -1;
    stack->stack_size = size;
    return stack;
}

void destroy_stack(StateStack *stack) {
    if (stack) {
        free(stack->stack);
        free(stack);
    }
}

int push_stack(StateStack *stack, State state) {
    if (!stack || stack->stack_top >= stack->stack_size - 1) return 0;
    stack->stack[++stack->stack_top] = state;
    return 1;
}

int pop_stack(StateStack *stack, State *state) {
    if (!stack || stack->stack_top < 0 || !state) return 0;
    *state = stack->stack[stack->stack_top--];
    return 1;
}

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(sizeof(Transition) * MAX_TRANSITIONS);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->stack = create_stack(MAX_STATES);
    if (!sm->stack) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->num_transitions = 0;
    
    Transition transitions[] = {
        {STATE_A, '1', STATE_B},
        {STATE_A, '2', STATE_C},
        {STATE_B, '1', STATE_A},
        {STATE_B, '2', STATE_D},
        {STATE_C, '1', STATE_D},
        {STATE_C, '2', STATE_E},
        {STATE_D, '1', STATE_E},
        {STATE_D, '2', STATE_A},
        {STATE_E, '1', STATE_A},
        {STATE_E, '2', STATE_B}
    };
    
    int num = sizeof(transitions) / sizeof(Transition);
    if (num > MAX_TRANSITIONS) num = MAX_TRANSITIONS;
    
    for (int i = 0; i < num; i++) {
        sm->transitions[i] = transitions[i];
    }
    sm->num_transitions = num;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        destroy_stack(sm->stack);
        free(sm->transitions);
        free(sm);
    }
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return STATE_A;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            if (push_stack(sm->stack, sm->current_state)) {
                sm->current_state = sm->transitions[i].next;
            }
            return sm->current_state;
        }
    }
    return sm->current_state;
}

int undo_last_transition(StateMachine *sm) {
    if (!sm) return 0;
    State prev_state;
    if (pop_stack(sm->stack, &prev_state)) {
        sm->current_state = prev_state;
        return 1;
    }
    return 0;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    char input[MAX_INPUT_LEN];
    printf("State Machine Demo (Enter '1' or '2' for transitions, 'u' to undo, 'q' to quit)\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    
    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        } else if (input[0] == 'u' && (input[1] == '\n' || input[1] == '\0')) {
            if (undo_last_trans