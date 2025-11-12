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
    int processed_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event, const char *data);
State handle_reading_state(StateMachine *sm, Event event, const char *data);
State handle_processing_state(StateMachine *sm, Event event, const char *data);
State handle_finished_state(StateMachine *sm, Event event, const char *data);
State handle_error_state(StateMachine *sm, Event event, const char *data);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->processed_count = 0;
}

State handle_start_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                size_t data_len = strlen(data);
                if (data_len > 0 && data_len < MAX_INPUT_LEN - sm->buffer_len) {
                    strncat(sm->buffer, data, data_len);
                    sm->buffer_len += data_len;
                    printf("Data received: %s\n", data);
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                printf("Processing data: %s\n", sm->buffer);
                sm->processed_count++;
                
                if (sm->processed_count >= 3) {
                    return STATE_FINISHED;
                } else {
                    memset(sm->buffer, 0, sizeof(sm->buffer));
                    sm->buffer_len = 0;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing completed. Total items: %d\n", sm->processed_count);
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("Error state reached\n");
    return STATE_ERROR;
}

State process_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event, data);
        case STATE_READING:
            return handle_reading_state(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event, data);
        case STATE_FINISHED:
            return handle_finished_state(sm, event, data);
        case STATE_ERROR:
            return handle_error_state(sm, event, data);
        default:
            return STATE_ERROR;
    }
}

int get_user_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    
    printf("Enter data (or 'process' to process, 'quit' to exit): ");
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    StateMachine sm;
    char input_buffer[MAX_INPUT_LEN];
    
    initialize_state_machine(&sm);
    
    printf("State Machine Demo - Data Processor\n");
    process_event(&sm, EVENT_START, NULL);
    
    while (sm.current_state != STATE_FINISHED && sm.current_state != STATE_ERROR) {
        if (!get_user_input(input_buffer, sizeof(input_buffer)))