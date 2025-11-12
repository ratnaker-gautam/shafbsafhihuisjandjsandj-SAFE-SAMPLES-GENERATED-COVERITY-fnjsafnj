//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUT_LEN 64
#define STACK_SIZE 32
#define HEAP_BUF_SIZE 128

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

typedef struct {
    State data[STACK_SIZE];
    int top;
} StateStack;

void stack_init(StateStack *stack) {
    stack->top = -1;
}

int stack_push(StateStack *stack, State state) {
    if (stack->top >= STACK_SIZE - 1) {
        return 0;
    }
    stack->data[++stack->top] = state;
    return 1;
}

int stack_pop(StateStack *stack, State *state) {
    if (stack->top < 0) {
        return 0;
    }
    *state = stack->data[stack->top--];
    return 1;
}

StateMachine* state_machine_create(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = STATE_A;
    return sm;
}

void state_machine_destroy(StateMachine *sm) {
    free(sm);
}

int state_machine_add_state(StateMachine *sm, State state) {
    if (sm->state_count >= MAX_STATES) {
        return 0;
    }
    sm->states[sm->state_count++] = state;
    return 1;
}

int state_machine_add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State state_machine_process(StateMachine *sm, char input) {
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

int main(void) {
    StateMachine *sm = state_machine_create();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    for (State s = STATE_A; s <= STATE_H; s++) {
        if (!state_machine_add_state(sm, s)) {
            fprintf(stderr, "Failed to add state\n");
            state_machine_destroy(sm);
            return 1;
        }
    }

    char *heap_buffer = malloc(HEAP_BUF_SIZE);
    if (!heap_buffer) {
        fprintf(stderr, "Failed to allocate heap buffer\n");
        state_machine_destroy(sm);
        return 1;
    }

    StateStack stack;
    stack_init(&stack);

    state_machine_add_transition(sm, STATE_A, '0', STATE_B);
    state_machine_add_transition(sm, STATE_A, '1', STATE_C);
    state_machine_add_transition(sm, STATE_B, '0', STATE_D);
    state_machine_add_transition(sm, STATE_B, '1', STATE_E);
    state_machine_add_transition(sm, STATE_C, '0', STATE_F);
    state_machine_add_transition(sm, STATE_C, '1', STATE_G);
    state_machine_add_transition(sm, STATE_D, '0', STATE_H);
    state_machine_add_transition(sm, STATE_D, '1', STATE_A);
    state_machine_add_transition(sm, STATE_E, '0', STATE_B);
    state_machine_add_transition(sm, STATE_E, '1', STATE_C);
    state_machine_add_transition(sm, STATE_F, '0', STATE_D);
    state_machine_add_transition(sm, STATE_F, '1', STATE_E);
    state_machine_add_transition(sm, STATE_G, '0', STATE_F);
    state_machine_add_transition(sm, STATE_G, '1', STATE_G);
    state_machine_add_transition(sm, STATE_H, '0', STATE_H);
    state_machine_add_transition(sm, STATE_H, '1', STATE_A);

    printf("Enter input sequence (0s and 1s, max %d chars): ", MAX_INPUT_LEN);
    if (!fgets(heap_buffer, HEAP_BUF_SIZE, stdin)) {
        fprintf(stderr, "Failed to read input\n");
        free(heap_buffer);
        state_machine_destroy(sm);
        return 1;
    }

    size_t len = strlen(heap_buffer);
    if (len > 0 && heap_buffer[len - 1] == '\n') {