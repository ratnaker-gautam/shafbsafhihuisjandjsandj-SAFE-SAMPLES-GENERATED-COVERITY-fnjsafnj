//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    char name[32];
    void (*action)(void);
} StateInfo;

typedef struct {
    Transition *transitions;
    StateInfo *states;
    int transition_count;
    int state_count;
    State current_state;
} StateMachine;

void idle_action(void) {
    printf("System is idle\n");
}

void active_action(void) {
    printf("System is active\n");
}

void paused_action(void) {
    printf("System is paused\n");
}

void error_action(void) {
    printf("System encountered an error\n");
}

void done_action(void) {
    printf("System completed task\n");
}

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transition_count = 0;
    sm->state_count = 5;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->states = malloc(MAX_STATES * sizeof(StateInfo));
    if (!sm->states) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->states[0].state = STATE_IDLE;
    strncpy(sm->states[0].name, "IDLE", 31);
    sm->states[0].name[31] = '\0';
    sm->states[0].action = idle_action;
    
    sm->states[1].state = STATE_ACTIVE;
    strncpy(sm->states[1].name, "ACTIVE", 31);
    sm->states[1].name[31] = '\0';
    sm->states[1].action = active_action;
    
    sm->states[2].state = STATE_PAUSED;
    strncpy(sm->states[2].name, "PAUSED", 31);
    sm->states[2].name[31] = '\0';
    sm->states[2].action = paused_action;
    
    sm->states[3].state = STATE_ERROR;
    strncpy(sm->states[3].name, "ERROR", 31);
    sm->states[3].name[31] = '\0';
    sm->states[3].action = error_action;
    
    sm->states[4].state = STATE_DONE;
    strncpy(sm->states[4].name, "DONE", 31);
    sm->states[4].name[31] = '\0';
    sm->states[4].action = done_action;
    
    sm->current_state = STATE_IDLE;
    
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_IDLE, EVENT_START, STATE_ACTIVE};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_STOP, STATE_IDLE};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_ERROR, STATE_ERROR};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_PAUSED, EVENT_STOP, STATE_IDLE};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_ERROR, EVENT_START, STATE_IDLE};
    }
    if (sm->transition_count < MAX_TRANSITIONS) {
        sm->transitions[sm->transition_count++] = (Transition){STATE_DONE, EVENT_START, STATE_IDLE};
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->states);
        free(sm);
    }
}

int process_event(StateMachine *sm, Event event) {
    if (!sm) return 0;
    
    for (int i =