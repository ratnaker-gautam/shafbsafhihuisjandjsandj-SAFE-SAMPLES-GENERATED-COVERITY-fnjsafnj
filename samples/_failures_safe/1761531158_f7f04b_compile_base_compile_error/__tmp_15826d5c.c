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
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_WRITE,
    EVENT_ERROR,
    EVENT_COMPLETE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_idle(StateMachine *sm, Event event, const char *data);
State handle_reading(StateMachine *sm, Event event, const char *data);
State handle_processing(StateMachine *sm, Event event, const char *data);
State handle_writing(StateMachine *sm, Event event, const char *data);
State handle_error(StateMachine *sm, Event event, const char *data);
State handle_done(StateMachine *sm, Event event, const char *data);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State handle_idle(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (strlen(data) < MAX_INPUT_LEN && sm->buffer_len + strlen(data) < MAX_INPUT_LEN) {
                strncpy(sm->buffer + sm->buffer_len, data, MAX_INPUT_LEN - sm->buffer_len - 1);
                sm->buffer[sm->buffer_len + strlen(data)] = '\0';
                sm->buffer_len += strlen(data);
                sm->data_count++;
                printf("Data received: %s\n", data);
                return STATE_READING;
            } else {
                printf("Buffer full or data too long\n");
                return STATE_ERROR;
            }
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("Processing %d data items\n", sm->data_count);
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE:
            printf("Data processed successfully\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Data written: %s\n", sm->buffer);
            printf("Processing complete\n");
            return STATE_DONE;
        case EVENT_ERROR:
            printf("Error during writing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Can only reset from ERROR state\n");
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Can only reset from DONE state\n");
            return STATE_DONE;
    }
}

void process_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return;
    
    State (*handlers[])(StateMachine*, Event, const char*) = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done