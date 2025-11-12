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
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine* sm, Event event);
State handle_reading_state(StateMachine* sm, Event event);
State handle_processing_state(StateMachine* sm, Event event);
State handle_done_state(StateMachine* sm, Event event);
State handle_error_state(StateMachine* sm, Event event);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

Event get_next_event(const char* input, size_t len) {
    if (input == NULL) return EVENT_ERROR;
    
    if (len == 0) return EVENT_START;
    
    if (strncmp(input, "DONE", 4) == 0 && len >= 4) return EVENT_COMPLETE;
    
    if (strncmp(input, "RESET", 5) == 0 && len >= 5) return EVENT_RESET;
    
    if (strncmp(input, "ERROR", 5) == 0 && len >= 5) return EVENT_ERROR;
    
    return EVENT_DATA;
}

State handle_start_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Reading data...\n");
                sm->data_count++;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            printf("Data reading complete\n");
            return STATE_PROCESSING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Processing data item %d\n", sm->data_count);
            if (sm->data_count > 0) {
                sm->data_count--;
            }
            return STATE_PROCESSING;
        case EVENT_COMPLETE:
            if (sm->data_count == 0) {
                printf("All data processed\n");
                return STATE_DONE;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_done_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_DONE;
    }
}

State handle_error_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            printf("Error state - reset required\n");
            return STATE_ERROR;
    }
}

State process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_DONE:
            return handle_done_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf