//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    char buffer[MAX_INPUT_LENGTH];
    size_t data_length;
    int error_code;
} StateMachine;

State handle_start(StateMachine* sm, Event event);
State handle_reading(StateMachine* sm, Event event);
State handle_processing(StateMachine* sm, Event event);
State handle_writing(StateMachine* sm, Event event);
State handle_error(StateMachine* sm, Event event);
State handle_done(StateMachine* sm, Event event);

typedef State (*StateHandler)(StateMachine*, Event);

StateHandler handlers[] = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error,
    handle_done
};

State handle_start(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            sm->data_length = 0;
            sm->error_code = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data to process (max %d chars): ", MAX_INPUT_LENGTH - 1);
            
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
                sm->error_code = 1;
                return STATE_ERROR;
            }
            
            sm->data_length = strlen(sm->buffer);
            if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
                sm->buffer[sm->data_length - 1] = '\0';
                sm->data_length--;
            }
            
            if (sm->data_length == 0) {
                printf("No data entered\n");
                return STATE_ERROR;
            }
            
            printf("Read %zu characters\n", sm->data_length);
            return STATE_PROCESSING;
            
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine* sm, Event event) {
    if (sm == NULL || sm->data_length == 0) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            printf("Processing data: '");
            for (size_t i = 0; i < sm->data_length; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    putchar(sm->buffer[i] - 32);
                } else {
                    putchar(sm->buffer[i]);
                }
            }
            printf("'\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data processing complete. Result: %s\n", sm->buffer);
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_error(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return handle_start(sm, EVENT_START);
        default:
            printf("Error state: code %d\n", sm->error_code);
            return STATE_ERROR;
    }
}

State handle_done(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            return handle_start(sm, EVENT_START);
        default:
            printf("Processing complete. Type 'reset' to start over or 'quit' to exit: ");
            return STATE_DONE;
    }
}

void process_event(StateMachine* sm, Event event) {
    if (sm == NULL || sm->current_state >= sizeof(handlers)/sizeof(handlers[0])) {
        return;
    }
    
    State new_state = handlers[sm->current_state](sm, event);
    sm->current_state = new_state;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.data_length = 0;
    sm.error_code = 0;
    memset(sm.buffer, 0, sizeof(sm