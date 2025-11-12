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
    STATE_COMPLETED,
    STATE_ERROR
} SystemState;

typedef struct {
    SystemState from_state;
    SystemState to_state;
    int trigger;
} Transition;

typedef struct {
    SystemState current_state;
    Transition *transitions;
    size_t transition_count;
    int *event_queue;
    size_t queue_size;
    size_t queue_capacity;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->queue_size = 0;
    sm->queue_capacity = 8;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->event_queue = malloc(sm->queue_capacity * sizeof(int));
    if (!sm->event_queue) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->event_queue);
        free(sm);
    }
}

int add_transition(StateMachine *sm, SystemState from, SystemState to, int trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

int enqueue_event(StateMachine *sm, int event) {
    if (!sm || event < 0) return 0;
    
    if (sm->queue_size >= sm->queue_capacity) {
        size_t new_capacity = sm->queue_capacity * 2;
        if (new_capacity > 1024) return 0;
        
        int *new_queue = realloc(sm->event_queue, new_capacity * sizeof(int));
        if (!new_queue) return 0;
        
        sm->event_queue = new_queue;
        sm->queue_capacity = new_capacity;
    }
    
    sm->event_queue[sm->queue_size++] = event;
    return 1;
}

int process_event(StateMachine *sm, int event) {
    if (!sm) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == event) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void process_queued_events(StateMachine *sm) {
    if (!sm) return;
    
    for (size_t i = 0; i < sm->queue_size; i++) {
        process_event(sm, sm->event_queue[i]);
    }
    sm->queue_size = 0;
}

const char* state_to_string(SystemState state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_COMPLETED: return "COMPLETED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, 1) ||
        !add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 2) ||
        !add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 3) ||
        !add_transition(sm, STATE_ACTIVE, STATE_COMPLETED, 4) ||
        !add_transition(sm, STATE_ACTIVE, STATE_ERROR, 5) ||
        !add_transition(sm, STATE_ERROR, STATE_IDLE, 6)) {
        fprintf(stderr, "Failed to add transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    int events[] = {1, 2, 3, 4};
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        if (!enqueue_event(sm, events[i])) {
            fprintf(stderr, "Failed to enqueue event\n");
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    process_queued_events(sm);
    printf("Final state: %s\n", state_to_string(sm->current_state