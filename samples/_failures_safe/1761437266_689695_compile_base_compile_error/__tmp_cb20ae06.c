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
    if (size <= 0 || size > MAX_STATES) return NULL;
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

StateMachine* create_state_machine(Transition *transitions, int num_trans) {
    if (!transitions || num_trans <= 0 || num_trans > MAX_TRANSITIONS) return NULL;
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(sizeof(Transition) * num_trans);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    memcpy(sm->transitions, transitions, sizeof(Transition) * num_trans);
    sm->num_transitions = num_trans;
    sm->current_state = STATE_A;
    
    sm->stack = create_stack(MAX_STATES);
    if (!sm->stack) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
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
    if (!sm) return NUM_STATES;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            
            if (input == 'p' && push_stack(sm->stack, sm->current_state)) {
                sm->current_state = sm->transitions[i].next;
                return sm->current_state;
            }
            
            if (input == 'b') {
                State prev_state;
                if (pop_stack(sm->stack, &prev_state)) {
                    sm->current_state = sm->transitions[i].next;
                    return sm->current_state;
                }
            }
            
            if (input != 'p' && input != 'b') {
                sm->current_state = sm->transitions[i].next;
                return sm->current_state;
            }
        }
    }
    return sm->current_state;
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "UNKNOWN";
    }
}

int main() {
    Transition transitions[] = {
        {STATE_A, '1', STATE_B},
        {STATE_A, '2', STATE_C},
        {STATE_B, 'p', STATE_C},
        {STATE_B, '3', STATE_D},
        {STATE_C, '4', STATE_E},
        {STATE_C, 'b', STATE_B},
        {STATE_D, '5', STATE_A},
        {STATE_E, 'b', STATE_C}
    };
    
    int num_trans = sizeof(transitions) / sizeof(transitions[0]);
    StateMachine *sm = create_state_machine(transitions, num_trans);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Simulation\n");
    printf("Available inputs: 1,2,3,4,5,p (push),b (back)\n");
    printf("Current state: A\n");
    
    char input[MAX_INPUT_LEN];
    while (1) {
        printf("Enter input (or 'q' to quit): ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) break;
        
        if (strlen(input) != 2 ||