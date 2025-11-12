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

void process_buffer(StateMachine *sm) {
    if (!sm || !sm->buffer) return;
    
    size_t len = strnlen(sm->buffer, sm->buffer_size - 1);
    if (len > 0) {
        printf("Processing buffer: %s\n", sm->buffer);
        memset(sm->buffer, 0, sm->buffer_size);
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    char input[BUFFER_SIZE];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, success, error, reset, quit\n");
    
    while (running) {
        printf("Current state: ");
        switch (sm->current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_ACTIVE: printf("ACTIVE"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            case STATE_DONE: printf("DONE"); break;
        }
        printf("\n> ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        EventType event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "data") == 0) {
            event = EVENT_DATA;
            if (sm->current_state == STATE_ACTIVE) {
                printf("Enter data: ");
                if (fgets(sm->buffer, sm->buffer_size, stdin)) {
                    sm->buffer[strcspn(sm->buffer, "\n")] = 0;
                }
            }
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
            if (sm->current_state == STATE_PROCESSING) {
                process_buffer(sm);
            }
        } else if (strcmp(input, "success") == 0) {