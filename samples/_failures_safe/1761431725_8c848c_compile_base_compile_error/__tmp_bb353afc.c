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
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR
} State;

typedef struct {
    State current;
    State next;
    char trigger;
} Transition;

typedef struct {
    State *state_stack;
    int stack_top;
    int stack_size;
} StateStack;

typedef struct {
    Transition *transitions;
    int transition_count;
    int max_transitions;
} StateMachine;

StateStack* create_stack(int size) {
    if (size <= 0 || size > 1000) return NULL;
    StateStack *stack = malloc(sizeof(StateStack));
    if (!stack) return NULL;
    stack->state_stack = malloc(sizeof(State) * size);
    if (!stack->state_stack) {
        free(stack);
        return NULL;
    }
    stack->stack_top = -1;
    stack->stack_size = size;
    return stack;
}

int push_state(StateStack *stack, State state) {
    if (!stack || stack->stack_top >= stack->stack_size - 1) return 0;
    stack->stack_top++;
    stack->state_stack[stack->stack_top] = state;
    return 1;
}

int pop_state(StateStack *stack, State *state) {
    if (!stack || !state || stack->stack_top < 0) return 0;
    *state = stack->state_stack[stack->stack_top];
    stack->stack_top--;
    return 1;
}

StateMachine* create_machine(int max_trans) {
    if (max_trans <= 0 || max_trans > MAX_TRANSITIONS) return NULL;
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    sm->transitions = malloc(sizeof(Transition) * max_trans);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    sm->transition_count = 0;
    sm->max_transitions = max_trans;
    return sm;
}

int add_transition(StateMachine *sm, State current, State next, char trigger) {
    if (!sm || sm->transition_count >= sm->max_transitions) return 0;
    sm->transitions[sm->transition_count].current = current;
    sm->transitions[sm->transition_count].next = next;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_transition(StateMachine *sm, State current, char trigger) {
    if (!sm) return STATE_ERROR;
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == current && sm->transitions[i].trigger == trigger) {
            return sm->transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void cleanup_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

void cleanup_stack(StateStack *stack) {
    if (stack) {
        free(stack->state_stack);
        free(stack);
    }
}

int main() {
    StateMachine *machine = create_machine(8);
    if (!machine) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    StateStack *stack = create_stack(5);
    if (!stack) {
        fprintf(stderr, "Failed to create state stack\n");
        cleanup_machine(machine);
        return 1;
    }

    if (!add_transition(machine, STATE_IDLE, STATE_READING, 'r') ||
        !add_transition(machine, STATE_READING, STATE_PROCESSING, 'p') ||
        !add_transition(machine, STATE_PROCESSING, STATE_WRITING, 'w') ||
        !add_transition(machine, STATE_WRITING, STATE_IDLE, 'i') ||
        !add_transition(machine, STATE_READING, STATE_ERROR, 'e') ||
        !add_transition(machine, STATE_PROCESSING, STATE_ERROR, 'e') ||
        !add_transition(machine, STATE_WRITING, STATE_ERROR, 'e') ||
        !add_transition(machine, STATE_ERROR, STATE_IDLE, 'r')) {
        fprintf(stderr, "Failed to add transitions\n");
        cleanup_machine(machine);
        cleanup_stack(stack);
        return 1;
    }

    State current_state = STATE_IDLE;
    char input[BUFFER_SIZE];

    printf("State Machine Started. Commands: r=read, p=process, w=write, i=idle, e=error, u=undo, q=quit\n");

    while (1) {
        printf("Current state: %d | Enter command: ", current_state);
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }

        if (input[0] == 'q') break;

        if (input[0] == 'u') {
            State prev_state;
            if (pop_state(stack, &prev_state)) {
                current_state = prev_state;
                printf("Undo to state: %d\n", current_state);
            } else {
                printf("No states to undo\n");
            }
            continue;
        }