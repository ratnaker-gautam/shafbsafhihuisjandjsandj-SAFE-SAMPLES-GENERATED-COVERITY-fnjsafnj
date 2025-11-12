//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_finished_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

Event get_next_event(StateMachine *sm) {
    if (sm == NULL) return EVENT_ERROR;
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        return EVENT_ERROR;
    }
    
    if (sm->current_state == STATE_START) {
        if (len > 0) {
            strncpy(sm->buffer, input, MAX_INPUT_LEN-1);
            sm->buffer[MAX_INPUT_LEN-1] = '\0';
            sm->buffer_len = len;
            return EVENT_START;
        }
    } else if (sm->current_state == STATE_READING) {
        if (strcmp(input, "process") == 0) {
            return EVENT_PROCESS;
        } else if (strcmp(input, "done") == 0) {
            return EVENT_COMPLETE;
        } else if (len > 0) {
            if (sm->buffer_len + len < MAX_INPUT_LEN-1) {
                strncat(sm->buffer, input, MAX_INPUT_LEN - sm->buffer_len - 1);
                sm->buffer_len += len;
                return EVENT_DATA;
            }
        }
    }
    
    return EVENT_ERROR;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data collection\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Data added. Current buffer: %s\n", sm->buffer);
            return STATE_READING;
        case EVENT_PROCESS:
            printf("Processing data: %s\n", sm->buffer);
            return STATE_PROCESSING;
        case EVENT_COMPLETE:
            printf("Data collection complete\n");
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Final data: %s\n", sm->buffer);
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    return STATE_FINISHED;
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    return STATE_ERROR;
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("State Machine Started\n");
    printf("Enter data (type 'process' to process, 'done' to finish):\n");
    
    while (sm->current_state != STATE_FINISHED && sm->current_state != STATE_ERROR) {
        Event event = get_next_event(sm);
        State next_state = sm->current_state;
        
        switch (sm->current_state) {
            case STATE_START:
                next_state = handle_start_state(sm, event);
                break;
            case STATE_READING:
                next_state = handle_reading_state(sm, event);
                break;
            case STATE_PROCESSING:
                next_state = handle_processing_state(sm, event);
                break;
            case STATE_FINISHED:
                next_state