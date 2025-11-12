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
    STATE_E
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State *stack;
    int top;
    int capacity;
} StateStack;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    StateStack *stack;
} StateMachine;

StateStack* create_stack(int capacity) {
    if (capacity <= 0) return NULL;
    StateStack *stack = malloc(sizeof(StateStack));
    if (!stack) return NULL;
    stack->stack = malloc(sizeof(State) * capacity);
    if (!stack->stack) {
        free(stack);
        return NULL;
    }
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void destroy_stack(StateStack *stack) {
    if (stack) {
        free(stack->stack);
        free(stack);
    }
}

int push(StateStack *stack, State state) {
    if (!stack || stack->top >= stack->capacity - 1) return 0;
    stack->stack[++stack->top] = state;
    return 1;
}

int pop(StateStack *stack, State *state) {
    if (!stack || stack->top < 0 || !state) return 0;
    *state = stack->stack[stack->top--];
    return 1;
}

StateMachine* create_state_machine(State initial, Transition *transitions, int count) {
    if (!transitions || count <= 0 || count > MAX_TRANSITIONS) return NULL;
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    sm->stack = create_stack(MAX_STATES);
    if (!sm->stack) {
        free(sm);
        return NULL;
    }
    sm->current_state = initial;
    sm->transitions = malloc(sizeof(Transition) * count);
    if (!sm->transitions) {
        destroy_stack(sm->stack);
        free(sm);
        return NULL;
    }
    memcpy(sm->transitions, transitions, sizeof(Transition) * count);
    sm->transition_count = count;
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
    if (!sm) return sm->current_state;
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            if (push(sm->stack, sm->current_state)) {
                sm->current_state = sm->transitions[i].next;
            }
            break;
        }
    }
    return sm->current_state;
}

int can_undo(StateMachine *sm) {
    return sm && sm->stack && sm->stack->top >= 0;
}

int undo(StateMachine *sm) {
    State prev_state;
    if (pop(sm->stack, &prev_state)) {
        sm->current_state = prev_state;
        return 1;
    }
    return 0;
}

void print_state(State state) {
    const char *names[] = {"A", "B", "C", "D", "E"};
    if (state >= STATE_A && state <= STATE_E) {
        printf("Current state: %s\n", names[state]);
    }
}

int main() {
    Transition transitions[] = {
        {STATE_A, '1', STATE_B},
        {STATE_A, '2', STATE_C},
        {STATE_B, '1', STATE_C},
        {STATE_B, '2', STATE_D},
        {STATE_C, '1', STATE_D},
        {STATE_C, '2', STATE_E},
        {STATE_D, '1', STATE_E},
        {STATE_D, '2', STATE_A},
        {STATE_E, '1', STATE_A},
        {STATE_E, '2', STATE_B}
    };
    
    int transition_count = sizeof(transitions) / sizeof(transitions[0]);
    StateMachine *sm = create_state_machine(STATE_A, transitions, transition_count);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[MAX_INPUT_LEN];
    printf("State Machine Simulation (Enter inputs: 1,2 or 'u' to undo, 'q' to quit)\n");
    
    while (1) {
        print_state(sm->current_state);
        printf("Input: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (input[0] == 'q' && input[1] == '\0') {
            break;
        } else if (input[0] == 'u' && input[1] == '\0') {
            if (can_undo(sm)) {
                undo(sm);
                printf("Undo performed\n");
            } else {
                printf("Cannot undo\n");
            }