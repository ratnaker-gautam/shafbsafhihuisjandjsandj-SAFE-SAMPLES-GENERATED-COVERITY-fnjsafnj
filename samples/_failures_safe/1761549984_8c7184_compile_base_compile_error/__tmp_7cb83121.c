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
    EVENT_COMPLETE
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
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
    sm->processed_count = 0;
}

State handle_idle(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (strlen(data) < MAX_INPUT_LEN) {
                strncpy(sm->buffer, data, MAX_INPUT_LEN - 1);
                sm->buffer[MAX_INPUT_LEN - 1] = '\0';
                sm->buffer_len = strlen(sm->buffer);
                printf("Read data: %s\n", sm->buffer);
                return STATE_PROCESSING;
            } else {
                printf("Input too long\n");
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                sm->processed_count++;
                printf("Processed: %s\n", sm->buffer);
                return STATE_WRITING;
            } else {
                printf("No data to process\n");
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE:
            printf("Output: %s\n", sm->buffer);
            if (sm->processed_count >= 3) {
                return STATE_DONE;
            } else {
                return STATE_READING;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            initialize_machine(sm);
            return STATE_READING;
        default:
            printf("In ERROR state, need START event to recover\n");
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_DONE;
    
    switch (event) {
        case EVENT_START:
            initialize_machine(sm);
            return STATE_READING;
        default:
            printf("Processing complete. Send START to begin again.\n");
            return STATE_DONE;
    }
}

State transition_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle(sm, event, data);
        case STATE_READING:
            return handle_reading(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing(sm, event, data);
        case STATE_WRITING:
            return handle_writing(sm, event, data);
        case STATE_ERROR:
            return handle_error(sm, event, data);