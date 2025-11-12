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
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_processed;
} StateMachine;

State handle_start(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine *, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error,
    handle_done
};

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            sm->buffer_len = 0;
            sm->data_processed = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
            
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
                return STATE_ERROR;
            }
            
            sm->buffer_len = strlen(sm->buffer);
            if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                sm->buffer[sm->buffer_len - 1] = '\0';
                sm->buffer_len--;
            }
            
            if (sm->buffer_len == 0) {
                printf("No data entered\n");
                return STATE_ERROR;
            }
            
            printf("Read %zu characters\n", sm->buffer_len);
            return STATE_PROCESSING;
            
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
            
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            printf("Processing data: '");
            for (size_t i = 0; i < sm->buffer_len && i < sizeof(sm->buffer) - 1; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
                putchar(sm->buffer[i]);
            }
            printf("'\n");
            
            sm->data_processed = 1;
            return STATE_WRITING;
            
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
            
        default:
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data written successfully\n");
            return STATE_DONE;
            
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
            
        default:
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return handle_start(sm, EVENT_START);
            
        default:
            printf("Error state - need reset\n");
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            return handle_start(sm, EVENT_START);
            
        default:
            printf("Processing complete - use reset to start over\n");
            return STATE_DONE;
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
        case STATE_ERROR:
        case STATE_DONE:
            return EVENT_RESET;
        default:
            return EVENT_ERROR;
    }
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    
    while (1)