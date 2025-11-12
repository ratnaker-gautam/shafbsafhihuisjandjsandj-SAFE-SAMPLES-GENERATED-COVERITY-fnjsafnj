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
} StateType;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_COMPLETE
} EventType;

typedef struct {
    StateType current_state;
    EventType event;
    StateType next_state;
} Transition;

typedef struct {
    StateType state;
    char name[32];
    void (*action)(void);
} StateInfo;

typedef struct {
    Transition *transitions;
    StateInfo *states;
    int transition_count;
    int state_count;
    StateType current_state;
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
    
    sm->states[0] = (StateInfo){STATE_IDLE, "IDLE", idle_action};
    sm->states[1] = (StateInfo){STATE_ACTIVE, "ACTIVE", active_action};
    sm->states[2] = (StateInfo){STATE_PAUSED, "PAUSED", paused_action};
    sm->states[3] = (StateInfo){STATE_ERROR, "ERROR", error_action};
    sm->states[4] = (StateInfo){STATE_DONE, "DONE", done_action};
    
    sm->transitions[sm->transition_count++] = (Transition){STATE_IDLE, EVENT_START, STATE_ACTIVE};
    sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED};
    sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_STOP, STATE_IDLE};
    sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_ERROR, STATE_ERROR};
    sm->transitions[sm->transition_count++] = (Transition){STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE};
    sm->transitions[sm->transition_count++] = (Transition){STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE};
    sm->transitions[sm->transition_count++] = (Transition){STATE_PAUSED, EVENT_STOP, STATE_IDLE};
    sm->transitions[sm->transition_count++] = (Transition){STATE_ERROR, EVENT_START, STATE_IDLE};
    sm->transitions[sm->transition_count++] = (Transition){STATE_DONE, EVENT_START, STATE_IDLE};
    
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

int process_event(StateMachine *sm, EventType event) {
    if (!sm || event < 0 || event >= MAX_EVENTS) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state && 
            sm->transitions[i].event == event) {
            
            sm->current_state = sm->transitions[i].next_state;
            
            for (int j = 0; j < sm->state_count; j++) {
                if (sm->states[j].state == sm->current_state && sm->states[j].action) {
                    sm->states[j].action();
                    return 1;
                }
            }
        }
    }
    
    printf("No valid transition from current state for this event\n");
    return 0;
}

void print_menu(void) {
    printf("\nState Machine Controller\n");
    printf("0: EVENT_START\n");
    printf("1: EVENT_PAUSE\n");
    printf("2: EVENT_RESUME\n");
    printf("3: EVENT_STOP\n");
    printf("4: EVENT_ERROR\n");
    printf("5: EVENT_COMPLETE\n");
    printf("6: Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine initialized in IDLE state\n");
    
    int running = 1;
    char input[BU