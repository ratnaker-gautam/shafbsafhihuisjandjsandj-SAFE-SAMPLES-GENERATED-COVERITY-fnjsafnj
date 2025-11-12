//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_IDLE,
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

State handle_idle(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine *, Event) = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error,
    handle_done
};

State handle_idle(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing...\n");
            sm->data_length = 0;
            sm->error_code = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
            
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
                sm->error_code = 1;
                return STATE_ERROR;
            }
            
            size_t len = strlen(sm->buffer);
            if (len > 0 && sm->buffer[len - 1] == '\n') {
                sm->buffer[len - 1] = '\0';
                len--;
            }
            
            if (len == 0) {
                printf("No data entered.\n");
                return STATE_READING;
            }
            
            sm->data_length = len;
            printf("Read %zu characters.\n", len);
            return STATE_PROCESSING;
            
        case EVENT_ERROR:
            sm->error_code = 2;
            return STATE_ERROR;
            
        case EVENT_RESET:
            return STATE_IDLE;
            
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_length == 0 || sm->data_length >= sizeof(sm->buffer)) {
                sm->error_code = 3;
                return STATE_ERROR;
            }
            
            printf("Processing data: '");
            for (size_t i = 0; i < sm->data_length; i++) {
                char c = sm->buffer[i];
                if (c >= 'a' && c <= 'z') {
                    putchar(c - 32);
                } else if (c >= 'A' && c <= 'Z') {
                    putchar(c + 32);
                } else {
                    putchar(c);
                }
            }
            printf("'\n");
            
            return STATE_WRITING;
            
        case EVENT_ERROR:
            sm->error_code = 4;
            return STATE_ERROR;
            
        case EVENT_RESET:
            return STATE_IDLE;
            
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data processing complete.\n");
            return STATE_DONE;
            
        case EVENT_ERROR:
            sm->error_code = 5;
            return STATE_ERROR;
            
        case EVENT_RESET:
            return STATE_IDLE;
            
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state (code: %d)\n", sm->error_code);
            sm->error_code = 0;
            return STATE_IDLE;
            
        default:
            printf("Error state active (code: %d). Use reset to continue.\n", sm->error_code);
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state.\n");
            return STATE_IDLE;
            
        default:
            printf("Processing complete. Use reset to start over.\n");
            return STATE_DONE;
    }
}

void process_event(StateMachine *sm, Event event) {
    if (sm ==