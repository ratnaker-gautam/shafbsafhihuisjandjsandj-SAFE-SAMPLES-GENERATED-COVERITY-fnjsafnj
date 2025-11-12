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
            memset(sm->buffer, 0, MAX_INPUT_LEN);
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
            
            if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) == NULL) {
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
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_length == 0 || sm->data_length >= MAX_INPUT_LEN) {
                sm->error_code = 2;
                return STATE_ERROR;
            }
            
            for (size_t i = 0; i < sm->data_length; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            
            printf("Processed data: %s\n", sm->buffer);
            return STATE_WRITING;
            
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
            printf("Data written successfully\n");
            return STATE_DONE;
            
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
            return handle_start(sm, EVENT_START);
            
        default:
            printf("Error occurred (code: %d)\n", sm->error_code);
            return STATE_ERROR;
    }
}

State handle_done(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return handle_start(sm, EVENT_START);
            
        default:
            printf("Processing complete. Use reset to start over.\n");
            return STATE_DONE;
    }
}

void run_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    Event current_event = EVENT_START;
    
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        if (sm->current_state < 0 || sm->current_state >= (State)(sizeof(handlers)/sizeof(handlers[0]))) {
            sm->current_state = STATE_ERROR;
            break;
        }
        
        StateHandler handler = handlers[sm->current_state];
        State next_state = handler(sm, current_event);
        sm