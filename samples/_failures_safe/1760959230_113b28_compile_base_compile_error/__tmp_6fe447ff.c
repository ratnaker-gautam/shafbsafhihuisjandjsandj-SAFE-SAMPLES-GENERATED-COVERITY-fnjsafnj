//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LENGTH];
    size_t data_length;
    int error_code;
} StateMachine;

State handle_start(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine *, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_done,
    handle_error
};

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->data_length = 0;
    sm->error_code = 0;
}

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started. Enter data: ");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                sm->data_length = strnlen(sm->buffer, sizeof(sm->buffer));
                if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
                    sm->buffer[sm->data_length - 1] = '\0';
                    sm->data_length--;
                }
                if (sm->data_length > 0) {
                    printf("Data read: %s\n", sm->buffer);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_length > 0 && sm->data_length < sizeof(sm->buffer)) {
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed data: %s\n", sm->buffer);
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data written successfully.\n");
            return STATE_DONE;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

Event get_next_event(State current_state) {
    switch (current_state) {
        case STATE_START:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA_READY;
        case STATE_PROCESSING:
            return EVENT_PROCESS_COMPLETE;
        case STATE_WRITING:
            return EVENT_WRITE_COMPLETE;
        case STATE_DONE:
            return EVENT_RESET;
        case STATE_ERROR:
            return EVENT_RESET;
        default:
            return EVENT_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    int max_iterations = 100;
    int iteration = 0;
    
    while (iteration < max_iterations) {
        Event event = get_next_event(sm.current_state);
        
        if (event == EVENT_ERROR) {
            sm.current_state = STATE_ERROR;
            printf("Invalid event for current state.\n");
            break;
        }
        
        if (sm.current_state >= 0 && sm.current_state <