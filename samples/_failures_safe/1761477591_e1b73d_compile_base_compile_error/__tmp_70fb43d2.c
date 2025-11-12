//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_EVENTS 10
#define MAX_NAME_LEN 32

typedef struct {
    char name[MAX_NAME_LEN];
    int id;
} State;

typedef struct {
    char name[MAX_NAME_LEN];
    int id;
} Event;

typedef struct {
    int from_state;
    int event_id;
    int to_state;
} Transition;

typedef struct {
    State *states;
    Event *events;
    Transition *transitions;
    int current_state;
    int num_states;
    int num_events;
    int num_transitions;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(MAX_STATES * sizeof(State));
    sm->events = malloc(MAX_EVENTS * sizeof(Event));
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    
    if (!sm->states || !sm->events || !sm->transitions) {
        free(sm->states);
        free(sm->events);
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->num_states = 0;
    sm->num_events = 0;
    sm->num_transitions = 0;
    sm->current_state = -1;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    free(sm->states);
    free(sm->events);
    free(sm->transitions);
    free(sm);
}

int add_state(StateMachine *sm, const char *name, int id) {
    if (!sm || !name || id < 0 || sm->num_states >= MAX_STATES) return 0;
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    strncpy(sm->states[sm->num_states].name, name, MAX_NAME_LEN - 1);
    sm->states[sm->num_states].name[MAX_NAME_LEN - 1] = '\0';
    sm->states[sm->num_states].id = id;
    sm->num_states++;
    
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    
    return 1;
}

int add_event(StateMachine *sm, const char *name, int id) {
    if (!sm || !name || id < 0 || sm->num_events >= MAX_EVENTS) return 0;
    
    for (int i = 0; i < sm->num_events; i++) {
        if (sm->events[i].id == id) return 0;
    }
    
    strncpy(sm->events[sm->num_events].name, name, MAX_NAME_LEN - 1);
    sm->events[sm->num_events].name[MAX_NAME_LEN - 1] = '\0';
    sm->events[sm->num_events].id = id;
    sm->num_events++;
    
    return 1;
}

int add_transition(StateMachine *sm, int from_state, int event_id, int to_state) {
    if (!sm || from_state < 0 || event_id < 0 || to_state < 0) return 0;
    if (sm->num_transitions >= MAX_TRANSITIONS) return 0;
    
    int from_exists = 0, to_exists = 0, event_exists = 0;
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == from_state) from_exists = 1;
        if (sm->states[i].id == to_state) to_exists = 1;
    }
    
    for (int i = 0; i < sm->num_events; i++) {
        if (sm->events[i].id == event_id) event_exists = 1;
    }
    
    if (!from_exists || !to_exists || !event_exists) return 0;
    
    sm->transitions[sm->num_transitions].from_state = from_state;
    sm->transitions[sm->num_transitions].event_id = event_id;
    sm->transitions[sm->num_transitions].to_state = to_state;
    sm->num_transitions++;
    
    return 1;
}

int process_event(StateMachine *sm, int event_id) {
    if (!sm || event_id < 0) return 0;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].event_id == event_id) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    
    return 0;
}

void print_current_state(StateMachine *sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->