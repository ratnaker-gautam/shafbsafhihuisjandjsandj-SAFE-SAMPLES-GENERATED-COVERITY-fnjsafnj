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

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    free(sm->states);
    free(sm->events);
    free(sm->transitions);
    free(sm);
}

int add_state(StateMachine *sm, const char *name) {
    if (!sm || !name || sm->num_states >= MAX_STATES) return -1;
    
    State *state = &sm->states[sm->num_states];
    strncpy(state->name, name, MAX_NAME_LEN - 1);
    state->name[MAX_NAME_LEN - 1] = '\0';
    state->id = sm->num_states;
    
    if (sm->current_state == -1) {
        sm->current_state = sm->num_states;
    }
    
    return sm->num_states++;
}

int add_event(StateMachine *sm, const char *name) {
    if (!sm || !name || sm->num_events >= MAX_EVENTS) return -1;
    
    Event *event = &sm->events[sm->num_events];
    strncpy(event->name, name, MAX_NAME_LEN - 1);
    event->name[MAX_NAME_LEN - 1] = '\0';
    event->id = sm->num_events;
    
    return sm->num_events++;
}

int add_transition(StateMachine *sm, int from_state, int to_state, int event_id) {
    if (!sm || from_state < 0 || from_state >= sm->num_states || 
        to_state < 0 || to_state >= sm->num_states || 
        event_id < 0 || event_id >= sm->num_events ||
        sm->num_transitions >= MAX_TRANSITIONS) {
        return -1;
    }
    
    Transition *trans = &sm->transitions[sm->num_transitions];
    trans->from_state = from_state;
    trans->to_state = to_state;
    trans->event_id = event_id;
    
    return sm->num_transitions++;
}

int process_event(StateMachine *sm, int event_id) {
    if (!sm || event_id < 0 || event_id >= sm->num_events) return -1;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        Transition *trans = &sm->transitions[i];
        if (trans->from_state == sm->current_state && trans->event_id == event_id) {
            sm->current_state = trans->to_state;
            return 0;
        }
    }
    
    return -1;
}

void print_current_state(StateMachine *sm) {
    if (!sm || sm->current_state < 0 || sm->current_state >= sm->num_states) {
        printf("Invalid state machine\n");
        return;
    }
    
    printf("Current state: %s\n", sm->states[sm->current_state].name);
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    int state_ids[4];
    state_ids[0] = add_state(sm, "IDLE");
    state_ids[1] = add_state(sm, "RUNNING");
    state_ids[2] = add_state(sm, "PAUSED");
    state_ids[3] = add_state(sm, "STOPPED");
    
    int event_ids[4];
    event_ids[0] = add_event(sm, "START");
    event_ids[1] = add_event(sm, "PAUSE");
    event_ids[2] = add_event(sm, "RESUME");
    event_ids[3] = add_event(sm, "STOP");
    
    add_transition(sm, state_ids[0], state_ids[1], event_ids[0]);
    add_transition(sm, state_ids[1], state_ids[2], event_ids[1]);