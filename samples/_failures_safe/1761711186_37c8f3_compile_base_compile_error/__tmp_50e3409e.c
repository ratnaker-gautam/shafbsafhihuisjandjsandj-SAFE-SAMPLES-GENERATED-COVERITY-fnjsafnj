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

StateMachine* create_state_machine(int max_transitions) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    sm->transitions = malloc(sizeof(Transition) * max_transitions);
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
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        destroy_stack(sm->stack);
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->num_transitions >= MAX_TRANSITIONS) return 0;
    sm->transitions[sm->num_transitions].current = from;
    sm->transitions[sm->num_transitions].input = input;
    sm->transitions[sm->num_transitions].next = to;
    sm->num_transitions++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            if (push_stack(sm->stack, sm->current_state)) {
                sm->current_state = sm->transitions[i].next;
            }
            break;
        }
    }
    return sm->current_state;
}

int can_undo(StateMachine *sm) {
    return sm && sm->stack && sm->stack->stack_top >= 0;
}

int undo_transition(StateMachine *sm) {
    State prev_state;
    if (!can_undo(sm)) return 0;
    if (pop_stack(sm->stack, &prev_state)) {
        sm->current_state = prev_state;
        return 1;
    }
    return 0;
}

const char* state_name(State state) {
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
    StateMachine *sm = create_state_machine(MAX_TRANSITIONS);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_C, '4', STATE_D);
    add_transition(sm, STATE_D, '5', STATE_E);
    add_transition(sm, STATE_E, '6', STATE_A);

    char input[MAX_INPUT_LEN];
    printf("State Machine Simulation (Current: %s)\n", state_name(sm->current_state));
    printf("Commands: 1-6=transition, u=undo, q=quit\n");

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) break;
        
        if (input[0] == 'u' && (input[1] ==