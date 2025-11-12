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
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
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
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
    memset(sm->buffer, 0, BUFFER_SIZE);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, Event event, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current_state = from;
    sm->transitions[sm->transition_count].event = event;
    sm->transitions[sm->transition_count].next_state = to;
    sm->transition_count++;
    return 1;
}

State process_event(StateMachine *sm, Event event) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state && 
            sm->transitions[i].event == event) {
            sm->current_state = sm->transitions[i].next_state;
            return sm->current_state;
        }
    }
    return STATE_ERROR;
}

void handle_state_action(StateMachine *sm) {
    if (!sm) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            strncpy(sm->buffer, "System ready", sm->buffer_size - 1);
            break;
        case STATE_ACTIVE:
            strncpy(sm->buffer, "Processing data", sm->buffer_size - 1);
            break;
        case STATE_PROCESSING:
            strncpy(sm->buffer, "Computing results", sm->buffer_size - 1);
            break;
        case STATE_ERROR:
            strncpy(sm->buffer, "Error occurred", sm->buffer_size - 1);
            break;
        case STATE_DONE:
            strncpy(sm->buffer, "Operation complete", sm->buffer_size - 1);
            break;
        default:
            strncpy(sm->buffer, "Unknown state", sm->buffer_size - 1);
            break;
    }
    sm->buffer[sm->buffer_size - 1] = '\0';
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, EVENT_START, STATE_ACTIVE);
    add_transition(sm, STATE_ACTIVE, EVENT_DATA, STATE_PROCESSING);
    add_transition(sm, STATE_PROCESSING, EVENT_SUCCESS, STATE_DONE);
    add_transition(sm, STATE_PROCESSING, EVENT_ERROR, STATE_ERROR);
    add_transition(sm, STATE_ERROR, EVENT_RESET, STATE_IDLE);
    add_transition(sm, STATE_DONE, EVENT_RESET, STATE_IDLE);
    
    Event events[] = {EVENT_START, EVENT_DATA, EVENT_SUCCESS, EVENT_RESET};
    size_t event_count = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < event_count; i++) {
        State new_state = process_event(sm, events[i]);
        handle_state_action(sm);
        printf("Event %zu: State %d -> %s\n", i, new_state, sm->buffer);
        
        if (new_state == STATE_ERROR) {
            process_event(sm, EVENT_RESET);
            handle_state_action(sm);
            printf("Reset: State %d -> %s\n", sm->current_state, sm->buffer);
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}