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
    
    Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_ACTIVE},
        {STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED},
        {STATE_ACTIVE, EVENT_STOP, STATE_IDLE},
        {STATE_ACTIVE, EVENT_ERROR, STATE_ERROR},
        {STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE},
        {STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE},
        {STATE_PAUSED, EVENT_STOP, STATE_IDLE},
        {STATE_ERROR, EVENT_STOP, STATE_IDLE}
    };
    
    for (int i = 0; i < sm->transition_count; i++) {
        sm->transitions[i] = transitions[i];
    }
    
    StateInfo states[] = {
        {STATE_IDLE, "IDLE", idle_action},
        {STATE_ACTIVE, "ACTIVE", active_action},
        {STATE_PAUSED, "PAUSED", paused_action},
        {STATE_ERROR, "ERROR", error_action},
        {STATE_DONE, "DONE", done_action}
    };
    
    for (int i = 0; i < sm->state_count; i++) {
        sm->states[i] = states[i];
    }
    
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

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - PAUSE\n");
    printf("2 - RESUME\n");
    printf("3 - STOP\n");
    printf("4 - ERROR\n");
    printf("5 - COMPLETE\n");
    printf("6 - EXIT\n");
    printf("Choose an event: ");
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    int running = 1;
    char input[BUFFER_SIZE];
    
    while (running) {
        print_menu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice < 0 || choice > 6) {
            printf("Invalid choice. Please select 0-6.\n");
            continue;
        }
        
        if (choice == 6) {
            running =