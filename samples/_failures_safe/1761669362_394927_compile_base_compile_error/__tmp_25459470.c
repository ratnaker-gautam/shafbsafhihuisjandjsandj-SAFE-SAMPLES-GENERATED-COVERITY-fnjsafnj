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
    if (!stack || !state || stack->top < 0) return 0;
    *state = stack->stack[stack->top--];
    return 1;
}

StateMachine* create_state_machine(int max_transitions) {
    if (max_transitions <= 0) return NULL;
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
    sm->transition_count = 0;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        destroy_stack(sm->stack);
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State current, char input, State next) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    if (current < 0 || current >= NUM_STATES || next < 0 || next >= NUM_STATES) return 0;
    sm->transitions[sm->transition_count].current = current;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = next;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return STATE_A;
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            if (push(sm->stack, sm->current_state)) {
                sm->current_state = sm->transitions[i].next;
            }
            return sm->current_state;
        }
    }
    return sm->current_state;
}

int main() {
    StateMachine *sm = create_state_machine(MAX_TRANSITIONS);
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }

    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_B, '4', STATE_E);
    add_transition(sm, STATE_C, '5', STATE_D);
    add_transition(sm, STATE_C, '6', STATE_E);
    add_transition(sm, STATE_D, '7', STATE_A);
    add_transition(sm, STATE_E, '8', STATE_A);

    char input[MAX_INPUT_LEN];
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        destroy_state_machine(sm);
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }

    printf("Initial state: %d\n", sm->current_state);
    for (size_t i = 0; i < len; i++) {
        State prev = sm->current_state;
        State new_state = process_input(sm, input[i]);
        printf("Input '%c': %d -> %d\n", input[i], prev, new_state);
    }

    printf("Final state: %d\n", sm->current_state);
    
    State popped;
    printf("State stack: ");
    while (pop(sm->stack, &popped)) {
        printf("%d ", popped);