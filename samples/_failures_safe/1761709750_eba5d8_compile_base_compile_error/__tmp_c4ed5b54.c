//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_EVENTS 10
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_COMPLETE
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State *state_stack;
    int stack_top;
    int stack_size;
} StateStack;

typedef struct {
    Transition *transitions;
    int transition_count;
    int transition_capacity;
    State current_state;
    StateStack *stack;
} StateMachine;

StateStack* create_stack(int size) {
    if (size <= 0 || size > 1000) {
        return NULL;
    }
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
    if (!stack || stack->stack_top >= stack->stack_size - 1) {
        return 0;
    }
    stack->stack_top++;
    stack->state_stack[stack->stack_top] = state;
    return 1;
}

int pop_state(StateStack *stack, State *state) {
    if (!stack || !state || stack->stack_top < 0) {
        return 0;
    }
    *state = stack->state_stack[stack->stack_top];
    stack->stack_top--;
    return 1;
}

void destroy_stack(StateStack *stack) {
    if (stack) {
        free(stack->state_stack);
        free(stack);
    }
}

StateMachine* create_state_machine(int initial_state) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    sm->transition_capacity = MAX_TRANSITIONS;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_capacity);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    sm->transition_count = 0;
    sm->current_state = initial_state;
    sm->stack = create_stack(MAX_STATES);
    if (!sm->stack) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    return sm;
}

int add_transition(StateMachine *sm, State from, Event event, State to) {
    if (!sm || sm->transition_count >= sm->transition_capacity) {
        return 0;
    }
    if (from < 0 || from > STATE_DONE || to < 0 || to > STATE_DONE || event < 0 || event > EVENT_COMPLETE) {
        return 0;
    }
    sm->transitions[sm->transition_count].current_state = from;
    sm->transitions[sm->transition_count].event = event;
    sm->transitions[sm->transition_count].next_state = to;
    sm->transition_count++;
    return 1;
}

int process_event(StateMachine *sm, Event event) {
    if (!sm || event < 0 || event > EVENT_COMPLETE) {
        return 0;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state && sm->transitions[i].event == event) {
            if (!push_state(sm->stack, sm->current_state)) {
                return 0;
            }
            sm->current_state = sm->transitions[i].next_state;
            return 1;
        }
    }
    return 0;
}

int undo_last_transition(StateMachine *sm) {
    if (!sm) return 0;
    State prev_state;
    if (pop_state(sm->stack, &prev_state)) {
        sm->current_state = prev_state;
        return 1;
    }
    return 0;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        destroy_stack(sm->stack);
        free(sm->transitions);
        free(sm);
    }
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(Event event) {
    switch(event) {
        case EVENT_START: return "START";
        case EVENT_PAUSE: return "PAUSE";
        case EVENT_RESUME: return "RESUME";
        case EVENT_STOP: return "STOP";
        case EVENT_ERROR: return "ERROR";
        case EVENT_COMPLETE: return "COMPLETE";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine(STATE_IDLE);
    if (!sm) {