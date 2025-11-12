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
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    State next;
    char trigger[32];
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    size_t transition_count;
    char *buffer;
    size_t buffer_size;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer = malloc(sm->buffer_size);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    memset(sm->buffer, 0, sm->buffer_size);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, const char *trigger) {
    if (!sm || !trigger) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    size_t len = strlen(trigger);
    if (len >= 32) return 0;
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, const char *trigger) {
    if (!sm || !trigger) return sm ? sm->current_state : STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && strcmp(t->trigger, trigger) == 0) {
            sm->current_state = t->next;
            break;
        }
    }
    
    return sm->current_state;
}

void process_buffer(StateMachine *sm) {
    if (!sm || !sm->buffer) return;
    
    size_t len = strlen(sm->buffer);
    if (len == 0) return;
    
    char stack_buffer[BUFFER_SIZE];
    if (len >= BUFFER_SIZE) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    strncpy(stack_buffer, sm->buffer, BUFFER_SIZE - 1);
    stack_buffer[BUFFER_SIZE - 1] = '\0';
    
    for (size_t i = 0; i < len; i++) {
        if (stack_buffer[i] == 'S') {
            process_trigger(sm, "start");
        } else if (stack_buffer[i] == 'P') {
            process_trigger(sm, "pause");
        } else if (stack_buffer[i] == 'R') {
            process_trigger(sm, "resume");
        } else if (stack_buffer[i] == 'E') {
            process_trigger(sm, "error");
        } else if (stack_buffer[i] == 'F') {
            process_trigger(sm, "finish");
        }
    }
    
    memset(sm->buffer, 0, sm->buffer_size);
}

const char* state_to_string(State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start") ||
        !add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause") ||
        !add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume") ||
        !add_transition(sm, STATE_ACTIVE, STATE_DONE, "finish") ||
        !add_transition(sm, STATE_PAUSED, STATE_DONE, "finish") ||
        !add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error") ||
        !add_transition(sm, STATE_PAUSED, STATE_ERROR, "error")) {
        fprintf(stderr, "Failed to setup transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("Enter command sequence (S=start, P=pause, R=resume, E=error, F=finish