//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_END,
    EVENT_INVALID,
    EVENT_TIMEOUT
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
    int (*action)(void*);
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
    int error_count;
} StateMachine;

int action_start(void* context) {
    StateMachine* sm = (StateMachine*)context;
    sm->buffer_len = 0;
    sm->data_count = 0;
    return 0;
}

int action_read(void* context) {
    StateMachine* sm = (StateMachine*)context;
    if (sm->buffer_len < MAX_INPUT_LEN - 1) {
        sm->buffer[sm->buffer_len++] = 'A';
        sm->data_count++;
        return 0;
    }
    return -1;
}

int action_process(void* context) {
    StateMachine* sm = (StateMachine*)context;
    if (sm->buffer_len > 0) {
        for (size_t i = 0; i < sm->buffer_len; i++) {
            if (!isalpha(sm->buffer[i])) {
                return -1;
            }
        }
        return 0;
    }
    return -1;
}

int action_done(void* context) {
    StateMachine* sm = (StateMachine*)context;
    printf("Processing complete. Data count: %d\n", sm->data_count);
    return 0;
}

int action_error(void* context) {
    StateMachine* sm = (StateMachine*)context;
    sm->error_count++;
    printf("Error occurred. Total errors: %d\n", sm->error_count);
    return 0;
}

Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_start},
    {STATE_READING, EVENT_DATA, STATE_READING, action_read},
    {STATE_READING, EVENT_END, STATE_PROCESSING, NULL},
    {STATE_PROCESSING, EVENT_DATA, STATE_PROCESSING, action_process},
    {STATE_PROCESSING, EVENT_END, STATE_DONE, action_done},
    {STATE_READING, EVENT_INVALID, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_INVALID, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_START, STATE_READING, action_start},
    {STATE_DONE, EVENT_START, STATE_READING, action_start}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

Event get_next_event(void) {
    static const char* events[] = {"start", "data", "end", "invalid", "timeout"};
    static size_t event_index = 0;
    
    if (event_index >= 5) {
        event_index = 0;
    }
    
    Event event = (Event)event_index;
    event_index++;
    return event;
}

int process_event(StateMachine* sm, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == sm->current_state && 
            transitions[i].event == event) {
            
            if (transitions[i].action != NULL) {
                int result = transitions[i].action(sm);
                if (result != 0) {
                    return -1;
                }
            }
            
            sm->current_state = transitions[i].next_state;
            return 0;
        }
    }
    return -1;
}

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->data_count = 0;
    sm->error_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Simulation Started\n");
    
    for (int i = 0; i < 20; i++) {
        Event event = get_next_event();
        
        printf("Current state: %d, Processing event: %d\n", 
               sm.current_state, event);
        
        if (process_event(&sm, event) != 0) {
            printf("Failed to process event\n");
        }
        
        if (sm.current_state == STATE_DONE) {
            printf("Reached DONE state\n");
            break;
        }
    }
    
    printf("Final state: %d, Data processed: %d, Errors: %d\n",
           sm.current_state, sm.data_count, sm.error_count);
    
    return 0;
}