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
    State state;
    char* name;
    void* data;
} StateNode;

typedef struct {
    Transition* transitions;
    int transition_count;
    StateNode* states;
    int state_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->states = malloc(MAX_STATES * sizeof(StateNode));
    if (!sm->states) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->transition_count = 0;
    sm->state_count = 0;
    sm->current_state = STATE_IDLE;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].name) {
            free(sm->states[i].name);
        }
        if (sm->states[i].data) {
            free(sm->states[i].data);
        }
    }
    
    free(sm->transitions);
    free(sm->states);
    free(sm);
}

int add_transition(StateMachine* sm, State from, Event event, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current_state = from;
    sm->transitions[sm->transition_count].event = event;
    sm->transitions[sm->transition_count].next_state = to;
    sm->transition_count++;
    return 1;
}

int add_state(StateMachine* sm, State state, const char* name, void* data) {
    if (!sm || !name || sm->state_count >= MAX_STATES) return 0;
    
    sm->states[sm->state_count].state = state;
    sm->states[sm->state_count].name = malloc(strlen(name) + 1);
    if (!sm->states[sm->state_count].name) return 0;
    
    strcpy(sm->states[sm->state_count].name, name);
    
    if (data) {
        sm->states[sm->state_count].data = malloc(sizeof(int));
        if (!sm->states[sm->state_count].data) {
            free(sm->states[sm->state_count].name);
            return 0;
        }
        memcpy(sm->states[sm->state_count].data, data, sizeof(int));
    } else {
        sm->states[sm->state_count].data = NULL;
    }
    
    sm->state_count++;
    return 1;
}

State process_event(StateMachine* sm, Event event) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state &&
            sm->transitions[i].event == event) {
            sm->current_state = sm->transitions[i].next_state;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

const char* get_state_name(StateMachine* sm, State state) {
    if (!sm) return "UNKNOWN";
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].state == state) {
            return sm->states[i].name;
        }
    }
    
    return "UNKNOWN";
}

int main() {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    int idle_data = 100;
    int active_data = 200;
    
    if (!add_state(sm, STATE_IDLE, "IDLE", &idle_data)) {
        printf("Failed to add state\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    if (!add_state(sm, STATE_ACTIVE, "ACTIVE", &active_data)) {
        printf("Failed to add state\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    if (!add_state(sm, STATE_PAUSED, "PAUSED", NULL)) {
        printf("Failed to add state\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    if (!add_state(sm, STATE_ERROR, "ERROR", NULL)) {
        printf("Failed to add state\n");
        destroy_state_machine(sm);
        return 1;
    }