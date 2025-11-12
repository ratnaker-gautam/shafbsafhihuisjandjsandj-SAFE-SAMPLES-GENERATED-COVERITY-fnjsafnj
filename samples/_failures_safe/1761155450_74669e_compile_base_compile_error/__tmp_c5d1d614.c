//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_EVENTS 10
#define MAX_STACK_SIZE 100
#define MAX_HEAP_STRINGS 50

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_ERROR
} StateType;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_ERROR
} EventType;

typedef struct {
    StateType current_state;
    EventType event;
    StateType next_state;
} Transition;

typedef struct {
    StateType state;
    int data;
} StackFrame;

typedef struct {
    Transition *transitions;
    int transition_count;
    StackFrame *call_stack;
    int stack_top;
    char **heap_strings;
    int heap_string_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->call_stack = malloc(MAX_STACK_SIZE * sizeof(StackFrame));
    if (!sm->call_stack) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->heap_strings = malloc(MAX_HEAP_STRINGS * sizeof(char*));
    if (!sm->heap_strings) {
        free(sm->call_stack);
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->transition_count = 0;
    sm->stack_top = -1;
    sm->heap_string_count = 0;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->heap_string_count; i++) {
        free(sm->heap_strings[i]);
    }
    
    free(sm->heap_strings);
    free(sm->call_stack);
    free(sm->transitions);
    free(sm);
}

int add_transition(StateMachine *sm, StateType from, EventType event, StateType to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current_state = from;
    sm->transitions[sm->transition_count].event = event;
    sm->transitions[sm->transition_count].next_state = to;
    sm->transition_count++;
    return 1;
}

int push_stack(StateMachine *sm, StateType state, int data) {
    if (!sm || sm->stack_top >= MAX_STACK_SIZE - 1) return 0;
    
    sm->stack_top++;
    sm->call_stack[sm->stack_top].state = state;
    sm->call_stack[sm->stack_top].data = data;
    return 1;
}

int pop_stack(StateMachine *sm, StackFrame *frame) {
    if (!sm || sm->stack_top < 0 || !frame) return 0;
    
    *frame = sm->call_stack[sm->stack_top];
    sm->stack_top--;
    return 1;
}

char* add_heap_string(StateMachine *sm, const char *str) {
    if (!sm || !str || sm->heap_string_count >= MAX_HEAP_STRINGS) return NULL;
    
    size_t len = strlen(str);
    if (len > 100) return NULL;
    
    char *new_str = malloc(len + 1);
    if (!new_str) return NULL;
    
    strncpy(new_str, str, len);
    new_str[len] = '\0';
    sm->heap_strings[sm->heap_string_count] = new_str;
    sm->heap_string_count++;
    return new_str;
}

StateType process_event(StateMachine *sm, StateType current_state, EventType event) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == current_state && 
            sm->transitions[i].event == event) {
            return sm->transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

const char* state_to_string(StateType state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(EventType event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_PAUSE: return "PAUSE";
        case EVENT_RESUME: return "RESUME";
        case EVENT_STOP: return "STOP";
        case EVENT_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, EVENT_START, STATE_ACTIVE);
    add_transition(sm, STATE