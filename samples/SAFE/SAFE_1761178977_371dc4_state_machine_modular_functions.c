//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} StateMachine;

State handle_start(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_finished(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine*, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            memset(sm->buffer, 0, MAX_INPUT_LEN);
            sm->buffer_len = 0;
            sm->processed_count = 0;
            printf("State machine started.\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_len] = 'A';
                sm->buffer_len++;
                printf("Data added. Buffer size: %zu\n", sm->buffer_len);
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                printf("Processing %zu items.\n", sm->buffer_len);
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            sm->processed_count += sm->buffer_len;
            sm->buffer_len = 0;
            memset(sm->buffer, 0, MAX_INPUT_LEN);
            printf("Processing complete. Total processed: %d\n", sm->processed_count);
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return handle_start(sm, event);
        default:
            return STATE_FINISHED;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("Error state encountered.\n");
    return STATE_ERROR;
}

Event get_next_event(void) {
    static int call_count = 0;
    
    if (call_count >= 20) {
        return EVENT_INVALID;
    }
    
    int event_type = call_count % 6;
    call_count++;
    
    switch (event_type) {
        case 0: return EVENT_START;
        case 1: return EVENT_DATA;
        case 2: return EVENT_DATA;
        case 3: return EVENT_PROCESS;
        case 4: return EVENT_COMPLETE;
        case 5: return EVENT_DATA;
        default: return EVENT_INVALID;
    }
}

int main(void) {
    StateMachine sm;
    State current_state = STATE_START;
    Event event;
    
    memset(&sm, 0, sizeof(StateMachine));
    
    for (int i = 0; i < 15; i++) {
        event = get_next_event();
        
        if (event == EVENT_INVALID) {
            printf("Invalid event received.\n");
            break;
        }
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            printf("Invalid state detected.\n");
            break;
        }
        
        if (state_handlers[current_state] == NULL) {
            printf("No handler for state %d.\n", current_state);
            break;
        }
        
        State new_state = state_handlers[current_state](&sm, event);
        
        if (new_state < 0 || new_state >= MAX_STATES) {
            printf("Invalid new state transition.\n");
            break;
        }
        
        current_state = new_state;
        
        if (current_state == STATE_FINISHED) {
            printf("State machine finished successfully.\n");
            break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("State machine entered error state.\n");
            break;
        }
    }
    
    printf("Final state: %d, Processed count: %d\n", current_state, sm.processed_count);
    
    return 0;
}