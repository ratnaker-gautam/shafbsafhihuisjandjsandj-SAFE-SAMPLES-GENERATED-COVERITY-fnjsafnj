//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 20

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    
    Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READY},
        {STATE_READY, EVENT_DATA, STATE_PROCESSING},
        {STATE_PROCESSING, EVENT_PROCESS, STATE_PROCESSING},
        {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
        {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR},
        {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE}
    };
    
    int count = sizeof(transitions) / sizeof(transitions[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count] = transitions[i];
            sm->transition_count++;
        }
    }
}

int process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state &&
            sm->transitions[i].event == event) {
            sm->current_state = sm->transitions[i].next_state;
            return 1;
        }
    }
    return 0;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA: return "DATA";
        case EVENT_PROCESS: return "PROCESS";
        case EVENT_SUCCESS: return "SUCCESS";
        case EVENT_FAILURE: return "FAILURE";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int get_valid_event() {
    char input[MAX_INPUT_LENGTH];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return -1;
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return -2;
    
    return -1;
}

void print_menu() {
    printf("Current state: ");
    printf("\nAvailable events: start, data, process, success, failure, reset, quit\n");
    printf("Enter event: ");
}

int main() {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(sm.current_state));
        printf("Available events: start, data, process, success, failure, reset, quit\n");
        printf("Enter event: ");
        
        int event = get_valid_event();
        
        if (event == -2) {
            printf("Exiting...\n");
            break;
        }
        
        if (event == -1) {
            printf("Invalid event. Please try again.\n\n");
            continue;
        }
        
        if (process_event(&sm, event)) {
            printf("Transition successful. New state: %s\n\n", state_to_string(sm.current_state));
        } else {
            printf("Invalid transition from current state.\n\n");
        }
    }
    
    return 0;
}