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
    STATE_DONE,
    STATE_ERROR
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

State handle_start_state(StateMachine* sm, Event event);
State handle_reading_state(StateMachine* sm, Event event);
State handle_processing_state(StateMachine* sm, Event event);
State handle_writing_state(StateMachine* sm, Event event);
State handle_done_state(StateMachine* sm, Event event);
State handle_error_state(StateMachine* sm, Event event);

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->data_length = 0;
    sm->error_code = 0;
}

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length >= MAX_INPUT_LENGTH) return 0;
    for (size_t i = 0; i < length; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
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
        case STATE_WRITING:
            return handle_writing_state(sm, event);
        case STATE_DONE:
            return handle_done_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            sm->error_code = 1;
            return STATE_ERROR;
    }
}

State handle_start_state(StateMachine* sm, Event event) {
    if (event == EVENT_START) {
        printf("State machine started\n");
        return STATE_READING;
    }
    sm->error_code = 2;
    return STATE_ERROR;
}

State handle_reading_state(StateMachine* sm, Event event) {
    if (event == EVENT_DATA_READY) {
        char input[MAX_INPUT_LENGTH];
        printf("Enter data to process (max %d chars): ", MAX_INPUT_LENGTH - 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            sm->error_code = 3;
            return STATE_ERROR;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (!validate_input(input, len)) {
            sm->error_code = 4;
            return STATE_ERROR;
        }
        
        if (len >= sizeof(sm->buffer)) {
            sm->error_code = 5;
            return STATE_ERROR;
        }
        
        strncpy(sm->buffer, input, sizeof(sm->buffer) - 1);
        sm->buffer[sizeof(sm->buffer) - 1] = '\0';
        sm->data_length = len;
        
        printf("Read %zu characters\n", len);
        return STATE_PROCESSING;
    }
    
    sm->error_code = 6;
    return STATE_ERROR;
}

State handle_processing_state(StateMachine* sm, Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        if (sm->data_length == 0) {
            sm->error_code = 7;
            return STATE_ERROR;
        }
        
        for (size_t i = 0; i < sm->data_length; i++) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
        
        printf("Processed data: %s\n", sm->buffer);
        return STATE_WRITING;
    }
    
    sm->error_code = 8;
    return STATE_ERROR;
}

State handle_writing_state(StateMachine* sm, Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Final output: %s\n", sm->buffer);
        printf("Data length: %zu\n", sm->data_length);
        return STATE_DONE;
    }
    
    sm->error_code = 9;
    return STATE_ERROR;
}

State handle_done_state(StateMachine* sm, Event event) {
    if (event == EVENT_RESET) {
        initialize_state_machine(sm);
        return STATE_START;
    }
    return STATE_DONE;
}

State handle_error_state(StateMachine* sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Res