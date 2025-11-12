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
    char name[32];
    void (*action)(void);
} StateInfo;

typedef struct {
    Transition *transitions;
    int transition_count;
    StateInfo *states;
    int state_count;
    State current_state;
} StateMachine;

void idle_action(void) {
    printf("Idle state: Waiting for start event\n");
}

void active_action(void) {
    printf("Active state: Processing data\n");
}

void paused_action(void) {
    printf("Paused state: Operation suspended\n");
}

void error_action(void) {
    printf("Error state: An error occurred\n");
}

void done_action(void) {
    printf("Done state: Operation completed\n");
}

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(MAX_STATES * sizeof(StateInfo));
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm->states);
        free(sm);
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states[0] = (StateInfo){STATE_IDLE, "IDLE", idle_action};
    sm->states[1] = (StateInfo){STATE_ACTIVE, "ACTIVE", active_action};
    sm->states[2] = (StateInfo){STATE_PAUSED, "PAUSED", paused_action};
    sm->states[3] = (StateInfo){STATE_ERROR, "ERROR", error_action};
    sm->states[4] = (StateInfo){STATE_DONE, "DONE", done_action};
    
    sm->transition_count = 8;
    sm->transitions[0] = (Transition){STATE_IDLE, EVENT_START, STATE_ACTIVE};
    sm->transitions[1] = (Transition){STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED};
    sm->transitions[2] = (Transition){STATE_ACTIVE, EVENT_STOP, STATE_IDLE};
    sm->transitions[3] = (Transition){STATE_ACTIVE, EVENT_ERROR, STATE_ERROR};
    sm->transitions[4] = (Transition){STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE};
    sm->transitions[5] = (Transition){STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE};
    sm->transitions[6] = (Transition){STATE_PAUSED, EVENT_STOP, STATE_IDLE};
    sm->transitions[7] = (Transition){STATE_ERROR, EVENT_STOP, STATE_IDLE};
    
    sm->current_state = STATE_IDLE;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int process_event(StateMachine *sm, Event event) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state &&
            sm->transitions[i].event == event) {
            sm->current_state = sm->transitions[i].next_state;
            
            for (int j = 0; j < sm->state_count; j++) {
                if (sm->states[j].state == sm->current_state && sm->states[j].action) {
                    sm->states[j].action();
                    break;
                }
            }
            return 1;
        }
    }
    
    printf("Invalid transition from current state\n");
    return 0;
}

Event get_event_from_input(const char *input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "pause") == 0) return EVENT_PAUSE;
    if (strcmp(input, "resume") == 0) return EVENT_RESUME;
    if (strcmp(input, "stop") == 0) return EVENT_STOP;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    return -1;
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    printf("State Machine Demo\n");
    printf("Available events: start, pause, resume, stop, error, complete\n");
    printf("Type 'quit' to exit\n\n");
    
    sm->states[0].action();