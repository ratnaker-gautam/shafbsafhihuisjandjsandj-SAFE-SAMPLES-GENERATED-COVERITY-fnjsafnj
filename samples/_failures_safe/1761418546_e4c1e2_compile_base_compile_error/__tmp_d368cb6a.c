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
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State from_state;
    Event event;
    State to_state;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    size_t transition_count;
    char *buffer;
    size_t buffer_size;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, Event event, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].event = event;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transition_count++;
    return 1;
}

State process_event(StateMachine *sm, Event event) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].event == event) {
            sm->current_state = sm->transitions[i].to_state;
            return sm->current_state;
        }
    }
    return sm->current_state;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_IDLE, EVENT_START, STATE_ACTIVE);
    add_transition(sm, STATE_ACTIVE, EVENT_DATA, STATE_PROCESSING);
    add_transition(sm, STATE_PROCESSING, EVENT_SUCCESS, STATE_DONE);
    add_transition(sm, STATE_PROCESSING, EVENT_ERROR, STATE_ERROR);
    add_transition(sm, STATE_ERROR, EVENT_RESET, STATE_IDLE);
    add_transition(sm, STATE_DONE, EVENT_RESET, STATE_IDLE);
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(Event event) {
    switch(event) {
        case EVENT_START: return "START";
        case EVENT_DATA: return "DATA";
        case EVENT_PROCESS: return "PROCESS";
        case EVENT_SUCCESS: return "SUCCESS";
        case EVENT_ERROR: return "ERROR";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    printf("State Machine Simulation\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=ERROR, 5=RESET\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    
    char input[32];
    Event events[] = {EVENT_START, EVENT_DATA, EVENT_PROCESS, EVENT_SUCCESS, EVENT_ERROR, EVENT_RESET};
    
    while (1) {
        printf("\nEnter event number (0-5) or 'q' to quit: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        if (input[0] == 'q' || input[0] == 'Q') break;
        
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice < 0 || choice > 5) {
            printf("Event must be between 0 and 5\n");
            continue;
        }
        
        Event event = events[choice];
        State old_state = sm->current_state;
        State new_state = process_event(sm, event);