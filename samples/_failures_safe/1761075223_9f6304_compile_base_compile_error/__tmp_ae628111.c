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
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_ERROR
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
    printf("Performing idle actions\n");
}

void active_action(void) {
    printf("Performing active actions\n");
}

void paused_action(void) {
    printf("Performing paused actions\n");
}

void error_action(void) {
    printf("Performing error handling\n");
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
    
    sm->state_count = 4;
    sm->states = malloc(sizeof(StateInfo) * sm->state_count);
    if (!sm->states) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_ACTIVE},
        {STATE_IDLE, EVENT_ERROR, STATE_ERROR},
        {STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED},
        {STATE_ACTIVE, EVENT_STOP, STATE_IDLE},
        {STATE_ACTIVE, EVENT_ERROR, STATE_ERROR},
        {STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE},
        {STATE_PAUSED, EVENT_STOP, STATE_IDLE},
        {STATE_PAUSED, EVENT_ERROR, STATE_ERROR}
    };
    
    memcpy(sm->transitions, transitions, sizeof(transitions));
    
    StateInfo states[] = {
        {STATE_IDLE, "IDLE", idle_action},
        {STATE_ACTIVE, "ACTIVE", active_action},
        {STATE_PAUSED, "PAUSED", paused_action},
        {STATE_ERROR, "ERROR", error_action}
    };
    
    memcpy(sm->states, states, sizeof(states));
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
    printf("Commands: start, pause, resume, stop, error, quit\n");
    
    while (running) {
        print_current_state(sm);
        printf("Enter command: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "pause") == 0) {
            event = EVENT_PAUSE;
        } else if (strcmp(input, "resume") == 0) {
            event = EVENT_RESUME;
        } else if (strcmp(input, "stop") == 0) {
            event = EVENT_STOP;
        } else if (strcmp(input, "error") == 0) {
            event = EVENT_ERROR;
        } else if (strcmp(input, "quit") == 0) {
            running =