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
    printf("System completed operation\n");
}

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transition_count = 8;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states = malloc(sizeof(StateInfo) * sm->state_count);
    if (!sm->states) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->transitions[0] = (Transition){STATE_IDLE, EVENT_START, STATE_ACTIVE};
    sm->transitions[1] = (Transition){STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED};
    sm->transitions[2] = (Transition){STATE_ACTIVE, EVENT_STOP, STATE_IDLE};
    sm->transitions[3] = (Transition){STATE_ACTIVE, EVENT_ERROR, STATE_ERROR};
    sm->transitions[4] = (Transition){STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE};
    sm->transitions[5] = (Transition){STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE};
    sm->transitions[6] = (Transition){STATE_PAUSED, EVENT_STOP, STATE_IDLE};
    sm->transitions[7] = (Transition){STATE_ERROR, EVENT_STOP, STATE_IDLE};
    
    strcpy(sm->states[0].name, "IDLE");
    sm->states[0].state = STATE_IDLE;
    sm->states[0].action = idle_action;
    
    strcpy(sm->states[1].name, "ACTIVE");
    sm->states[1].state = STATE_ACTIVE;
    sm->states[1].action = active_action;
    
    strcpy(sm->states[2].name, "PAUSED");
    sm->states[2].state = STATE_PAUSED;
    sm->states[2].action = paused_action;
    
    strcpy(sm->states[3].name, "ERROR");
    sm->states[3].state = STATE_ERROR;
    sm->states[3].action = error_action;
    
    strcpy(sm->states[4].name, "DONE");
    sm->states[4].state = STATE_DONE;
    sm->states[4].action = done_action;
    
    sm->current_state = STATE_IDLE;
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
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state && 
            sm->transitions[i].event == event) {
            sm->current_state = sm->transitions[i].next_state;
            
            for (int j = 0; j < sm->state_count; j++) {
                if (sm->states[j].state == sm->current_state) {
                    sm->states[j].action();
                    return 1;
                }
            }
        }
    }
    return 0;
}

void print_current_state(StateMachine *sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].state == sm->current_state) {
            printf("Current state: %s\n", sm->states[i].name);
            return;
        }
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    int running = 1;
    
    printf("State Machine Controller\n");
    printf("Commands: start, pause, resume, stop, error, complete, quit\n");
    
    while (running) {