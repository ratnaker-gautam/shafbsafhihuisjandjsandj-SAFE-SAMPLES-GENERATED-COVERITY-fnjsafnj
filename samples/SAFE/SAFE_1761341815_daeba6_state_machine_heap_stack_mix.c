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

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transition_count = 8;
    sm->state_count = 4;
    
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
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
    return 0;
}

int get_event_from_input(const char *input) {
    if (!input) return -1;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "pause") == 0) return EVENT_PAUSE;
    if (strcmp(input, "resume") == 0) return EVENT_RESUME;
    if (strcmp(input, "stop") == 0) return EVENT_STOP;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    
    return -1;
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    printf("State Machine Controller\n");
    printf("Available commands: start, pause, resume, stop, error, quit\n");
    
    while (1) {
        printf("\nEnter command: ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        int event = get_event_from_input(input);
        if (event == -1) {
            printf("Invalid command\n");
            continue;
        }
        
        if (!process_event(sm, event)) {
            printf("Invalid transition from current state\n");
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}