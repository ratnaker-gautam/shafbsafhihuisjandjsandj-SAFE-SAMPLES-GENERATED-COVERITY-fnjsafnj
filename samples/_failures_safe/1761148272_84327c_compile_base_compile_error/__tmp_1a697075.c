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
    int to_state;
    int event_id;
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

int add_state(StateMachine *sm, const char *name) {
    if (!sm || !name || sm->num_states >= MAX_STATES) return -1;
    
    State *state = &sm->states[sm->num_states];
    strncpy(state->name, name, MAX_NAME_LEN - 1);
    state->name[MAX_NAME_LEN - 1] = '\0';
    state->id = sm->num_states;
    sm->num_states++;
    return state->id;
}

int add_event(StateMachine *sm, const char *name) {
    if (!sm || !name || sm->num_events >= MAX_EVENTS) return -1;
    
    Event *event = &sm->events[sm->num_events];
    strncpy(event->name, name, MAX_NAME_LEN - 1);
    event->name[MAX_NAME_LEN - 1] = '\0';
    event->id = sm->num_events;
    sm->num_events++;
    return event->id;
}

int add_transition(StateMachine *sm, int from_state, int to_state, int event_id) {
    if (!sm || from_state < 0 || to_state < 0 || event_id < 0) return -1;
    if (from_state >= sm->num_states || to_state >= sm->num_states || event_id >= sm->num_events) return -1;
    if (sm->num_transitions >= MAX_TRANSITIONS) return -1;
    
    Transition *trans = &sm->transitions[sm->num_transitions];
    trans->from_state = from_state;
    trans->to_state = to_state;
    trans->event_id = event_id;
    sm->num_transitions++;
    return 0;
}

int process_event(StateMachine *sm, int event_id) {
    if (!sm || event_id < 0 || event_id >= sm->num_events) return -1;
    if (sm->current_state < 0) return -1;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        Transition *trans = &sm->transitions[i];
        if (trans->from_state == sm->current_state && trans->event_id == event_id) {
            sm->current_state = trans->to_state;
            return 0;
        }
    }
    return -1;
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    free(sm->states);
    free(sm->events);
    free(sm->transitions);
    free(sm);
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    int idle = add_state(sm, "IDLE");
    int running = add_state(sm, "RUNNING");
    int paused = add_state(sm, "PAUSED");
    int stopped = add_state(sm, "STOPPED");
    
    int start_evt = add_event(sm, "START");
    int pause_evt = add_event(sm, "PAUSE");
    int resume_evt = add_event(sm, "RESUME");
    int stop_evt = add_event(sm, "STOP");
    
    add_transition(sm, idle, running, start_evt);
    add_transition(sm, running, paused, pause_evt);
    add_transition(sm, paused, running, resume_evt);
    add_transition(sm, running, stopped, stop_evt);
    add_transition(sm, paused, stopped, stop_evt);
    
    sm->current_state = idle;
    
    printf("State machine initialized. Current state: %s\n", sm->states[sm->current_state].name);
    
    int events[] = {start_evt, pause_evt, resume_evt