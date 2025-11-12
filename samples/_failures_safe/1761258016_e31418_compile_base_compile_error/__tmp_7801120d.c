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
} StateType;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_ERROR,
    EVENT_RESET
} EventType;

typedef struct {
    StateType from_state;
    EventType event;
    StateType to_state;
} Transition;

typedef struct {
    StateType current_state;
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

int add_transition(StateMachine *sm, StateType from, EventType event, StateType to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].event = event;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transition_count++;
    return 1;
}

StateType process_event(StateMachine *sm, EventType event) {
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

int read_input(char *buffer, size_t size) {
    if (!buffer || size == 0) return 0;
    
    printf("Enter event (start, data, process, success, error, reset, quit): ");
    
    if (fgets(buffer, size, stdin) == NULL) return 0;
    
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

EventType parse_event(const char *input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return -1;
}

const char* state_name(StateType state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
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
    
    char input_buffer[BUFFER_SIZE];
    int running = 1;
    
    printf("State Machine Simulation\n");
    printf("Current state: %s\n", state_name(sm->current_state));
    
    while (running) {
        if (!read_input(input_buffer, sizeof(input_buffer))) {
            fprintf(stderr, "Failed to read input\n");
            break;
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            running = 0;
            continue;
        }
        
        EventType event = parse_event(input_buffer);
        if (event == -1) {
            printf("Invalid event. Try again.\n");
            continue;