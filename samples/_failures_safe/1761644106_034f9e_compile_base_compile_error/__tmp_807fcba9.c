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

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
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

State handle_idle_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_START:
            if (data != NULL && validate_input(data, data_len)) {
                if (data_len < sizeof(machine->buffer)) {
                    memcpy(machine->buffer, data, data_len);
                    machine->buffer_len = data_len;
                    machine->data_count = 1;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, data_len)) {
                if (machine->buffer_len + data_len < sizeof(machine->buffer)) {
                    memcpy(machine->buffer + machine->buffer_len, data, data_len);
                    machine->buffer_len += data_len;
                    machine->data_count++;
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_PROCESS:
            if (machine->buffer_len > 0) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_WRITE:
            if (machine->buffer_len > 0) {
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_writing_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_COMPLETE:
            if (machine->buffer_len > 0) {
                printf("Processed %d data chunks: %.*s\n", 
                       machine->data_count, (int)machine->buffer_len, machine->buffer);
                return STATE_DONE;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

State process_event(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            return handle_idle_state(machine, event, data, data_len);
        case STATE_READING:
            return handle_reading_state(machine, event, data, data_len);
        case STATE_PROCESSING:
            return handle_processing_state(machine, event, data, data_len);
        case STATE_WRITING:
            return handle_writing_state(machine, event, data, data_len);
        case STATE_ERROR:
            return handle_error_state(machine, event, data, data_len);
        case STATE_DONE:
            return handle_done_state