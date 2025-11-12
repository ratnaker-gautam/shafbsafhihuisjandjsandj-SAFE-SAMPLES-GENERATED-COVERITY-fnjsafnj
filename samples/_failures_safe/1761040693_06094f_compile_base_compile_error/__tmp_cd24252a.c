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

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_len = 0;
    machine->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (!(input[i] >= 'a' && input[i] <= 'z') &&
            !(input[i] >= 'A' && input[i] <= 'Z') &&
            !(input[i] >= '0' && input[i] <= '9') &&
            input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

State handle_start_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            if (data != NULL && data_len > 0 && validate_input(data, data_len)) {
                if (data_len < sizeof(machine->buffer)) {
                    memcpy(machine->buffer, data, data_len);
                    machine->buffer_len = data_len;
                    machine->data_count = 1;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && data_len > 0 && validate_input(data, data_len)) {
                size_t new_len = machine->buffer_len + data_len;
                if (new_len < sizeof(machine->buffer) && machine->data_count < MAX_STATES) {
                    memcpy(machine->buffer + machine->buffer_len, data, data_len);
                    machine->buffer_len = new_len;
                    machine->data_count++;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            if (machine->buffer_len > 0) {
                for (size_t i = 0; i < machine->buffer_len; i++) {
                    if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
                        machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
                    }
                }
                return STATE_FINISHED;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            return handle_start_state(machine, event, data, data_len);
        case STATE_READING:
            return handle_reading_state(machine, event, data, data_len);
        case STATE_PROCESSING:
            return handle_processing_state(machine, event, data, data_len);
        case STATE_FINISHED:
            return handle_finished_state(machine, event, data, data_len);
        case STATE_ERROR:
            return handle_error_state(machine, event, data, data_len);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(StateMachine *machine) {
    if (machine == NULL) return;
    
    printf("Current state