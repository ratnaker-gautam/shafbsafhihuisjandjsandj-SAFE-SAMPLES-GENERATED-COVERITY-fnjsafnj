//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_EVENTS 5
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
    EVENT_ERROR,
    EVENT_COMPLETE
} EventType;

typedef struct {
    StateType current_state;
    EventType event;
    StateType next_state;
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
    memset(sm->buffer, 0, BUFFER_SIZE);
    
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
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current_state = from;
    t->event = event;
    t->next_state = to;
    sm->transition_count++;
    return 1;
}

StateType process_event(StateMachine *sm, EventType event) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current_state == sm->current_state && t->event == event) {
            sm->current_state = t->next_state;
            return sm->current_state;
        }
    }
    return STATE_ERROR;
}

int read_input(char *buffer, size_t size) {
    if (!buffer || size == 0) return 0;
    
    printf("Enter event (0=START, 1=DATA, 2=PROCESS, 3=ERROR, 4=COMPLETE): ");
    
    if (fgets(buffer, size, stdin) == NULL) return 0;
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || value < 0 || value > 4) return 0;
    
    return (int)value;
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
    
    add_transition(sm, STATE_IDLE, EVENT_START, STATE_ACTIVE);
    add_transition(sm, STATE_ACTIVE, EVENT_DATA, STATE_PROCESSING);
    add_transition(sm, STATE_PROCESSING, EVENT_PROCESS, STATE_DONE);
    add_transition(sm, STATE_PROCESSING, EVENT_ERROR, STATE_ERROR);
    add_transition(sm, STATE_ERROR, EVENT_START, STATE_IDLE);
    add_transition(sm, STATE_DONE, EVENT_COMPLETE, STATE_IDLE);
    
    printf("State Machine Simulation\n");
    printf("Current state: %s\n", state_name(sm->current_state));
    
    char input_buffer[32];
    int running = 1;
    
    while (running) {
        int event = read_input(input_buffer, sizeof(input_buffer));
        if (event < 0) {
            printf("Invalid input\n");
            continue;
        }
        
        StateType new_state = process_event(sm, (EventType)event);
        printf("Event processed. New state: %s\n", state_name(new_state));
        
        if (new_state == STATE_DONE) {
            printf("Process completed successfully\n");
            running = 0;
        } else if (new_state == STATE_ERROR) {
            printf("Error state reached\n");
            running = 0;
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}