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
    int data_processed;
} StateMachine;

State handle_idle_state(StateMachine *sm, Event event, const char *data);
State handle_reading_state(StateMachine *sm, Event event, const char *data);
State handle_processing_state(StateMachine *sm, Event event, const char *data);
State handle_writing_state(StateMachine *sm, Event event, const char *data);
State handle_error_state(StateMachine *sm, Event event, const char *data);
State handle_done_state(StateMachine *sm, Event event, const char *data);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_processed = 0;
}

State handle_idle_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("State: IDLE (reset)\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (strlen(data) < MAX_INPUT_LEN) {
                strncpy(sm->buffer, data, MAX_INPUT_LEN - 1);
                sm->buffer[MAX_INPUT_LEN - 1] = '\0';
                sm->buffer_len = strlen(sm->buffer);
                printf("State: READING -> PROCESSING (data: %s)\n", sm->buffer);
                return STATE_PROCESSING;
            } else {
                printf("Input too long\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("State: READING -> IDLE (reset)\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                sm->data_processed = 1;
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                printf("State: PROCESSING -> WRITING (processed: %s)\n", sm->buffer);
                return STATE_WRITING;
            } else {
                printf("No data to process\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("State: PROCESSING -> IDLE (reset)\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

State handle_writing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE:
            if (sm->data_processed && sm->buffer_len > 0) {
                printf("Output: %s\n", sm->buffer);
                printf("State: WRITING -> DONE\n");
                return STATE_DONE;
            } else {
                printf("No processed data to write\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("State: WRITING -> IDLE (reset)\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("State: ERROR -> IDLE (reset)\n");
            return STATE_IDLE;
        default:
            printf("Must reset from ERROR state\n");
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete\n");
            return STATE_DONE;
        case EVENT_RESET:
            printf("State: DONE -> IDLE (